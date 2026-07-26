#include <common.h>
#include <gamestates.h>
#include <save_state.h>
#include "menu/menu.h"


// Platform specific ram addresses for savestates
#if defined(VERSION10_PS1)
#define SAVE_STATE_MEMORY_ADDRESS 0x80300000 // For Vita
#elif defined(VERSION10_PS2)
#define SAVE_STATE_MEMORY_ADDRESS 0x80A10000 // For PS2 Deckard
#else
#define SAVE_STATE_MEMORY_ADDRESS 0x80400000 // For Duckstation testing
#endif

#define PARTICLE_POOL_POINTER_ADDRESS    0x8006C554
#define LEVEL_OBJECTS_POINTER_ADDRESS    0x800722C8
#define SCENE_REGION_TABLE_ADDRESS       0x8006D048
#define SCENE_REGION_COUNT_ADDRESS       0x8006D04C

#define PARTICLE_POOL_SIZE               0x2020
#define COLLISION_TRIANGLE_SIZE          0x0C
#define SCENE_REGION_ANIMATION_SIZE      4

bool hasLoadstatedDuringIL = false;

// IL Timer
extern u32 g_ILTimerMode;
extern u32 IL_mainTimerAtReset;
extern u32 ilTimerOffset;
extern u32 IL_mainTimerAtReset;

// Manual Timer
extern u32 mainTimerAtReset;

// Rhynoc Trainer
extern u32 input_timer_during_thrown_anim;
extern u32 rhynoc_currentButtonSequence;
extern u32 rhynoc_input_sequence[3];
extern u32 rhynoc_wait_to_show;


// Savestate regions
typedef struct SaveRegion
{
    void* address;
    u32 byteCount;
} SaveRegion;

#define SAVE_STATE_REGION(address, byteCount) { (void*)(address), byteCount }

static const SaveRegion saveStateRegions[] =
{
    // Layout
    SAVE_STATE_REGION(0x8006C514, 0x08),
    SAVE_STATE_REGION(0x8006C7E0, 0x04),
    SAVE_STATE_REGION(0x8006D048, 0x08),
    SAVE_STATE_REGION(0x8006D070, 0x04),
    SAVE_STATE_REGION(0x800722C8, 0x04),

    // Moby and particle pointers
    SAVE_STATE_REGION(0x8006C550, 0x08),
    SAVE_STATE_REGION(0x8006C574, 0x08),
    SAVE_STATE_REGION(0x8006C5EC, 0x04),
    SAVE_STATE_REGION(0x8006C610, 0x08),
    SAVE_STATE_REGION(0x8006C700, 0x08),
    SAVE_STATE_REGION(0x8006C710, 0x04),
    SAVE_STATE_REGION(0x8006C728, 0x0C),

    // HUD data
    SAVE_STATE_REGION(0x80067248, 0x300),
    SAVE_STATE_REGION(0x8006580C, 0x28),
    SAVE_STATE_REGION(0x80066BC8, 0x14),
    SAVE_STATE_REGION(0x8006C564, 0x04),
    SAVE_STATE_REGION(0x8006C584, 0x04),
    SAVE_STATE_REGION(0x8006C5F4, 0x04),
    SAVE_STATE_REGION(0x8006C628, 0x08),
    SAVE_STATE_REGION(0x8006C640, 0x10),
    SAVE_STATE_REGION(0x8006C660, 0x04),
    SAVE_STATE_REGION(0x8006C6C0, 0x08),
    SAVE_STATE_REGION(0x8006C6F8, 0x04),
    SAVE_STATE_REGION(0x8006C71C, 0x08),
    SAVE_STATE_REGION(0x8006C74C, 0x04),
    SAVE_STATE_REGION(0x8006C784, 0x04),
    SAVE_STATE_REGION(0x8006C79C, 0x1C),
    SAVE_STATE_REGION(0x8006C7C0, 0x08),

    // NPC dialogue state
    SAVE_STATE_REGION(0x8006C51C, 0x08),
    SAVE_STATE_REGION(0x8006C53C, 0x0C),
    SAVE_STATE_REGION(0x8006C57C, 0x04),
    SAVE_STATE_REGION(0x8006C590, 0x10),
    SAVE_STATE_REGION(0x8006C5A8, 0x08),
    SAVE_STATE_REGION(0x8006C5C4, 0x04),
    SAVE_STATE_REGION(0x8006C5FC, 0x04),
    SAVE_STATE_REGION(0x8006C608, 0x04),
    SAVE_STATE_REGION(0x8006C620, 0x08),
    SAVE_STATE_REGION(0x8006C66C, 0x04),
    SAVE_STATE_REGION(0x8006C674, 0x04),
    SAVE_STATE_REGION(0x8006C680, 0x04),
    SAVE_STATE_REGION(0x8006C684, 0x1C),
    SAVE_STATE_REGION(0x8006C6A4, 0x04),
    SAVE_STATE_REGION(0x8006C6D8, 0x08),
    SAVE_STATE_REGION(0x8006C6E4, 0x14),
    SAVE_STATE_REGION(0x8006C6FC, 0x04),
    SAVE_STATE_REGION(0x8006C738, 0x04),
    SAVE_STATE_REGION(0x8006C750, 0x04),
    SAVE_STATE_REGION(0x8006C76C, 0x04),
    SAVE_STATE_REGION(0x8006C770, 0x04),
    SAVE_STATE_REGION(0x8006C778, 0x04),
    SAVE_STATE_REGION(0x8006C788, 0x14),
    SAVE_STATE_REGION(0x8006C7BC, 0x04),
    SAVE_STATE_REGION(0x8006C7D0, 0x04),

    // Dialogue line
    SAVE_STATE_REGION(0x80071390, 0x68),
    SAVE_STATE_REGION(0x80071938, 0x28),

    // Current and saved checkpoint data
    SAVE_STATE_REGION(0x8006C7F8, 0x850),
    SAVE_STATE_REGION(0x8006D088, 0x850),

    // Camera data
    SAVE_STATE_REGION(0x8006DFF8, 0x1FC),
    SAVE_STATE_REGION(0x8006E390, 0xE0),
    SAVE_STATE_REGION(0x8006E498, 0x04),
    SAVE_STATE_REGION(0x8006E4F4, 0x10),

    // Spyro, side character, Sparx, and game state
    SAVE_STATE_REGION(0x8006FA2C, 0x54),
    SAVE_STATE_REGION(0x8006FB90, 0x38),
    SAVE_STATE_REGION(0x8006E344, 0x04), // gamestate
    SAVE_STATE_REGION(0x8006FBC8, 0x04), // menuState
    SAVE_STATE_REGION(0x8006FBCC, 0x04), // menuOption
    SAVE_STATE_REGION(0x8006FBD0, 0x04), // currentMenu
    SAVE_STATE_REGION(0x8006FBD4, 0x04),
    SAVE_STATE_REGION(0x8006E49C, 0x04), // pauseMusic
    SAVE_STATE_REGION(0x80070260, 0x34C), // Light effects?

    // Portal state
    SAVE_STATE_REGION(0x800722E8, 0x48),

    // flame, collision, and various hud states
    SAVE_STATE_REGION(0x8007145C, 0x28),
    SAVE_STATE_REGION(0x80071570, 0x22C),

    // Flame segments, hit flags, and other various flame stuff
    SAVE_STATE_REGION(0x800717AC, 0x130),
    SAVE_STATE_REGION(0x80071900, 0x38),
    SAVE_STATE_REGION(0x800719C8, 0x18),
    SAVE_STATE_REGION(0x80071A10, 0x5C8),

    // RNG. (Maybe make a toggle for this eventually? Could be nice to choose to have the same rng)
    // SAVE_STATE_REGION(0x80073F3C, 0x04),

    // Save our own rhynoc trainer data
    SAVE_STATE_REGION(&input_timer_during_thrown_anim, sizeof(input_timer_during_thrown_anim)),
    SAVE_STATE_REGION(&rhynoc_currentButtonSequence, sizeof(rhynoc_currentButtonSequence)),
    SAVE_STATE_REGION(&rhynoc_input_sequence, sizeof(rhynoc_input_sequence)),
    SAVE_STATE_REGION(&rhynoc_wait_to_show, sizeof(rhynoc_wait_to_show)),
};

#undef SAVE_STATE_REGION

typedef enum SaveStateOperation
{
    LOAD_STATE,
    SAVE_STATE
} SaveStateOperation;

typedef struct SaveStateHeader
{
    bool isValid;
    u32 level;
    u32 subLevel;
    u32 totalSize;
    u32 levelObjectsAddress;
    u32 levelDataSize;
} SaveStateHeader;

u32 saveStateStoredBytes = 0;

static bool IsSafeToUseSaveState(void)
{
    bool isNormalGameplay = gamestate == GAMEPLAY || gamestate == INTERACTING;
    bool isPortalExit = gamestate == LOADING_LEVEL && menuState == 5 && framesInScenario > 0;
    bool isStablePause = gamestate == PAUSED && menuState == 0;
    bool isValidLevel = currentLevel > 0 && currentLevel < 0x3C;

    return (isNormalGameplay || isPortalExit || isStablePause) && isValidLevel;
}

// Which direction to copy (save, or load)
static void TransferMemoryRegion(u8** saveStateCursor, void* gameAddress, u32 byteCount, SaveStateOperation operation)
{
    if (operation == SAVE_STATE)
        MemCopy(*saveStateCursor, gameAddress, byteCount);
    else
        MemCopy(gameAddress, *saveStateCursor, byteCount);

    *saveStateCursor += byteCount;
}


static u8* TransferSaveStateData(SaveStateOperation operation)
{
    SaveStateHeader* header = (SaveStateHeader*)SAVE_STATE_MEMORY_ADDRESS;
    u8* saveStateCursor = (u8*)SAVE_STATE_MEMORY_ADDRESS + sizeof(SaveStateHeader);
    u32 levelObjectsAddress = MEM32(LEVEL_OBJECTS_POINTER_ADDRESS);
    u32 particlePoolAddress = MEM32(PARTICLE_POOL_POINTER_ADDRESS);
    u32 levelDataSize = particlePoolAddress + PARTICLE_POOL_SIZE - levelObjectsAddress;
    u32* sceneRegionAddresses;
    u32 sceneRegionCount;
    CollisionHeader* collision;
    u32 i;

    if (operation == SAVE_STATE)
    {
        header->levelObjectsAddress = levelObjectsAddress;
        header->levelDataSize = levelDataSize;
    }
    else
    {
        levelObjectsAddress = header->levelObjectsAddress;
        levelDataSize = header->levelDataSize;
    }

    // Level mobys, special data, collision, and particles.
    TransferMemoryRegion(&saveStateCursor, (void*)levelObjectsAddress, levelDataSize, operation);

    // Copy every region from the array of pointers
    for (i = 0; i < ARRAY_SIZE(saveStateRegions); i++)
    {
        const SaveRegion* region = &saveStateRegions[i];

        TransferMemoryRegion(&saveStateCursor, region->address, region->byteCount, operation);
    }

    // Moving collision data
    collision = TerrainCollisionData;
    sceneRegionAddresses = (u32*)MEM32(SCENE_REGION_TABLE_ADDRESS);
    sceneRegionCount = MEM32(SCENE_REGION_COUNT_ADDRESS);

    TransferMemoryRegion(&saveStateCursor, collision->triangles, collision->tri_count * COLLISION_TRIANGLE_SIZE, operation);

    for (i = 0; i < sceneRegionCount; i++)
    {
        TransferMemoryRegion(&saveStateCursor, (void*)(sceneRegionAddresses[i] + 0x18), SCENE_REGION_ANIMATION_SIZE, operation);
    }

    return saveStateCursor;
}

bool SaveStateCapture(void)
{
    SaveStateHeader* header = (SaveStateHeader*)SAVE_STATE_MEMORY_ADDRESS;
    u8* saveStateEnd;

    if (!IsSafeToUseSaveState())
    {
        // Saving during loading gamestate/cutscene, don't allow
        return false;
    }

    header->isValid = false;
    header->level = currentLevel;
    header->subLevel = subLevelID;

    saveStateEnd = TransferSaveStateData(SAVE_STATE);
    header->totalSize = (u32)saveStateEnd - SAVE_STATE_MEMORY_ADDRESS;
    header->isValid = true;

    saveStateStoredBytes = header->totalSize;

    //printf_syscall("Size: %X\n", saveStateEnd);
    return true;
}

bool SaveStateRestore(void)
{
    SaveStateHeader* header = (SaveStateHeader*)SAVE_STATE_MEMORY_ADDRESS;

    if (!header->isValid)
    {
        // No save state
        return false;
    }

    if (!IsSafeToUseSaveState())
    {
        // Not in a good gamestate
        return false;
    }

    if (header->level != currentLevel || header->subLevel != subLevelID)
    {
        // Not in the same level anymore
        return false;
    }

    if (header->totalSize < sizeof(*header))
    {
        return false;
    }

    TransferSaveStateData(LOAD_STATE);

    saveStateStoredBytes = header->totalSize;
    return true;
}

void FullSaveState(void)
{
    SaveStateCapture();


    if (g_ILTimerMode > 0) // If IL mode is on (I should really start using the enum for this...)
    {
        ilTimerOffset = globalTimer - IL_mainTimerAtReset; // Save IL timer
    }
}

void FullLoadState(void)
{
    SaveStateRestore();

    if (g_ILTimerMode > 0) // If IL mode is on (I should really start using the enum for this...)
    {
        hasLoadstatedDuringIL = true;

        IL_mainTimerAtReset = globalTimer - ilTimerOffset;
    }

    mainTimerAtReset = globalTimer; // Reset manual timer on load state
}
