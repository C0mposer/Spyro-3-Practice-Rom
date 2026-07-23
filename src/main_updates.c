#include "menu/menu.h"
#include <buttons.h>
#include <gamestates.h>
#include <hotkeys.h>
#include <level_ids.h>
#include <symbols.h>
#include <syscalls.h>
#include <types.h>
#include <upgrades.h>

enum ReloadLevelType
{
    RELOAD_FULL,
    RELOAD_CHECKPOINT,
    RELOAD_BALLOON
};

extern Vec3 savedStartingPosition;
extern Vec3 savedStartingPositionSubLevel;
extern u32 savedStartingAngle;
extern u32 savedStartingAngleSubLevel;
extern bool speedUpResetPending;
extern bool hasSavedSpyroPosition;

extern Vec3 savedSpyroPosition;
extern s32 savedSpyroYawAngle;
extern u32 savedPositionLevelID;
extern u32 savedPositionSubLevelID;
extern u8 savedSpyroSwimState; // For saved pos
extern u8 startingSpyroSwimState; // For level entry
extern u8 startingSpyroSwimStateSubLevel; // For sub level entry

extern u32 reloadSpyroTimer;

extern Menu main_menu;

inline static void SpeedUpReset(void)
{
    const u32 minimumLayoutWait = 6;
    const u32 normalDeathLayoutWait = 0x50;

    // Skip the artificial death delay
    if (speedUpResetPending && gamestate == DYING && menuState == 1 &&
        framesInScenario >= minimumLayoutWait)
    {
        framesInScenario = normalDeathLayoutWait;
        speedUpResetPending = false;
    }
}

typedef enum FastLoadFadeMode {
    FAST_LOAD_CUT_BOTH_FADES,
    FAST_LOAD_KEEP_FADE_IN
} FastLoadFadeMode;

bool fastLoadActive = false;
bool fastLoadInScenario = false;
static FastLoadFadeMode fastLoadFadeMode = FAST_LOAD_CUT_BOTH_FADES;

bool FastLoadEnabled(void)
{
    // printf_syscall("FAST_LOAD_TOGGLE = %d, Enabled = %d\n", FAST_LOAD_TOGGLE,
    // main_menu.elements[FAST_LOAD_TOGGLE].enabled);
    return main_menu.elements[FAST_LOAD_TOGGLE].enabled != 0;
}

inline static void FastLoadUpdate(void)
{
    const u32 fadeFullBlack = 0xFF; // Phase 0: jump straight to black
    const u32 fadeInFinishStep =
        0x10; // Phase 4: game subtracts 0x10 then completes at < 1
    const u32 skipArtificialWait = 0x50;

    if (!fastLoadActive) { return; }

    if (gamestate == DYING)
    {
        fastLoadInScenario = true;

        switch (menuState)
        {
        case 0: // Force full black so the game advances immediately
            drawScreenBlack = fadeFullBlack;
            break;
        case 1: // Skip the artificial delay
            if (framesInScenario != 0 && framesInScenario < skipArtificialWait)
            {
                framesInScenario = skipArtificialWait;
            }
            break;
        case 4: // fade-in: Only cut fade in for the saved-position teleport.
                // The level-start reset keeps it so enemy cycles are accurate.
            if (fastLoadFadeMode == FAST_LOAD_CUT_BOTH_FADES &&
                drawScreenBlack > fadeInFinishStep)
            {
                drawScreenBlack = fadeInFinishStep;
            }
            break;
        default:
            break;
        }
    }
    else if (fastLoadInScenario && gamestate == GAMEPLAY)
    {
        // Returned to gameplay, reset bools
        fastLoadActive = false;
        fastLoadInScenario = false;
        fastLoadFadeMode = FAST_LOAD_CUT_BOTH_FADES;
    }
}

inline static void SaveStartingPositionUpdate(void)
{
    static u32 localPreviousLevelID = 0;
    static u32 currentSubLevelID = 0;

    if (gamestate == GAMEPLAY)
    {
        if (localPreviousLevelID != currentLevel)
        {
            //printf_syscall("Saved Level ID\n");

            Vec3Copy(&savedStartingPosition, &respawnPosition); // Save the respawn position for the current level/sub level
            savedStartingAngle = respawnAngle; // Save the respawn position for the current level/sub level
            startingSpyroSwimState = savedCheckpointSwimState;

            localPreviousLevelID = currentLevel;
        }
        if (currentSubLevelID != subLevelID)
        {
            //printf_syscall("Saved Sublevel ID\n");
            Vec3Copy(&savedStartingPositionSubLevel, &respawnPosition); // Save the respawn position for the current level/sub level
            savedStartingAngleSubLevel = respawnAngle; // Save the respawn position for the current level/sub level
            startingSpyroSwimStateSubLevel = savedCheckpointSwimState;

            currentSubLevelID = subLevelID;
        }
    }
}

extern u32 g_ILTimerMode;
inline static void ReloadLevelStartingPosition(u32 reloadLevelType)
{
    if (subLevelID == 0)
    {
        Vec3Copy(&respawnPosition, &savedStartingPosition);
        respawnAngle = savedStartingAngle;
        savedCheckpointSwimState = startingSpyroSwimState;
    }
    else
    {
        Vec3Copy(&respawnPosition, &savedStartingPositionSubLevel);
        respawnAngle = savedStartingAngleSubLevel;
        savedCheckpointSwimState = startingSpyroSwimStateSubLevel;
    }

    if (reloadLevelType == RELOAD_CHECKPOINT)
    {
        previousLevelIDForVehicleEntry = 0;
        savedCheckpointUpdated = 1;
    }
    if (reloadLevelType == RELOAD_FULL)
    {
        savedCheckpointUpdated = 1;
        previousLevelIDForVehicleEntry = 0;

        if (currentLevel == BUZZS_DUNGEON) // Always enter on balloon in buzz
        {
            previousLevelIDForVehicleEntry = 40;
        }
    }
    else if (reloadLevelType == RELOAD_BALLOON)
    {
        previousLevelIDForVehicleEntry = 40; // Is a valid HW id (% 10), but not a real HW
    }
}

inline static bool HasSavedSpyroPositionForCurrentLevel(void)
{
    return hasSavedSpyroPosition && savedPositionLevelID == currentLevel && savedPositionSubLevelID == subLevelID;
}

// Manually save spyro's position
inline static void ManualSaveSpyroPositionUpdate(void)
{
    const u32 saveCombo = SAVE_SPYRO_HOTKEY;
    const u32 savedMessageDuration = 60;
    static bool saveComboWasHeld = false;
    static bool showSavedMessage = false;
    static u32 savedMessageStartTime = 0;
    bool saveComboIsHeld = (isButtonHeld == saveCombo);

    if (saveComboIsHeld && !saveComboWasHeld)
    {
        savedSpyroPosition.x = spyroX;
        savedSpyroPosition.y = spyroY;
        savedSpyroPosition.z = spyroZ;
        savedSpyroYawAngle = spyroYawAngle;

        savedPositionLevelID = currentLevel;
        savedPositionSubLevelID = subLevelID;

        if (spyroState == 10) // Over water swim
        {
            savedSpyroSwimState = 1;
        }
        else if (spyroState == 11 || spyroState == 12) // Under water swim
        {
            savedSpyroSwimState = 2;
        }
        else
        {
            savedSpyroSwimState = 0;
        }

        hasSavedSpyroPosition = true;
        savedMessageStartTime = levelTimer;
        showSavedMessage = true;
    }

    saveComboWasHeld = saveComboIsHeld;

    if (showSavedMessage)
    {
        if ((u32)(levelTimer - savedMessageStartTime) < savedMessageDuration)
        {
            DrawText("Saved", 10, 215, 0x1, 0);
        }
        else
        {
            showSavedMessage = false;
        }
    }
}

void PrepareSavedSpyroRespawn(void)
{
    if (HasSavedSpyroPositionForCurrentLevel())
    {
        // Set spyro's respawn position, to the saved position
        savedCheckpointUpdated = true;
        Vec3Copy(&respawnPosition, &savedSpyroPosition);
        respawnAngle = (u32)savedSpyroYawAngle;
        savedCheckpointSwimState = savedSpyroSwimState;

        previousLevelIDForVehicleEntry = 0; // Force no balloon for loading saved position

        PreserveSideCharacterForRespawn();
    }
    else
    {
        bool shouldLoadBalloon = false;
        ReloadLevelStartingPosition(shouldLoadBalloon);
        PreserveSideCharacterForRespawn();
    }
}

void RespawnSpyro(void)
{
    spyroZ = 0; // Kill spyro by sending him to the void
}

void RestartLevelFromBeginning(u32 reloadLevelType)
{
    ClearCollectables();
    ReloadLevelStartingPosition(reloadLevelType);
    PreserveSideCharacterForRespawn();

    speedUpResetPending = true;
    if (FastLoadEnabled())
    {
        fastLoadActive = true;
        fastLoadInScenario = false;
        fastLoadFadeMode = FAST_LOAD_KEEP_FADE_IN;
    }

    RespawnSpyro();
}

void UpdateHomeworldEntryType()
{
    if (gamestate == LOADING_CUTSCENE)
    {
        if (HasHeldButton(TRIANGLE_BUTTON, 20)) // Load in on balloon
        {
            previousLevelIDForVehicleEntry = 10; // Must be any hw ID (x % 10 essentially)
            //printf_syscall("Should be balloon\n");
        }
        else // Don't load in on balloon
        {
            previousLevelIDForVehicleEntry = 0;
            //printf_syscall("Should be NO balloon\n");
        }
    }
}

void ClearCollectables(void)
{
    u32 gem_reset_value = main_menu.elements[SET_GEM_COUNT_TOGGLE].value;
    u32 egg_reset_value = main_menu.elements[SET_EGG_COUNT_TOGGLE].value;
    // Clear Gem & Egg count
    globalGems = gem_reset_value;
    globalEggs = egg_reset_value;

    // Clear gem flags
    memset(gemsCollectedPerLevel, 0, 0xA0);
    memset(gemsCollectedFlags, 0, 0x500);

    // Clear egg flags
    memset(eggsCollectedBitmask, 0, 0x28);
    memset(eggDifficultyState, 0, 0xF0);
    memset(recordedPerformancePerEgg, 0, 0xF0);
    eggsCollectedSinceEnteredBitmask = 0;
    eggsCollectedThisVisit = 0;

    // Clear progress flags
    memset(progressFlags, 0, 0x4C);
    memset(dialogueFlags, 0, 0x28);
    memset(skillPointFlags, 0, 0x14);

    // Reset boss death counters (they can override adaptive difficulty
    // sometimes)
    deathsToBuzz = 0;
    deathsToSpike = 0;
    deathsToScorch = 0;
    deathsToSorceress = 0;

    // Reset Sparx data
    totalHits = 0;
    MemCopy(sparxState, sparxDefaults, 0x28);

    // Reset checkpoint data
    memset(currentCheckpointData, 0, 0x850);
    memset(savedCheckpointData, 0, 0x850);

}

inline static void UnlockAtlasWarp()
{
    memset(hasEnteredLevelFlags, 0x11111111, 40); // Unlock all levels in atlas
    upgradeFlags = 2; // Hacky because the overlays are encrypted, and doing the
                      // code below changes it at runtime, breaking the CRC. Fix
                      // eventually by patching the overlay directly.

    // Can't do this at runtime because of anti-tamper :)
    //     // Check if the atlas overlay is loaded. If so, nop the sparx
    //     breaking basket checks, to force atlas warp to work
    //     opcode_as_u32* check_1 = ((opcode_as_u32*)0x800791DC);
    //     opcode_as_u32* check_2 = ((opcode_as_u32*)0x8007755C);
    //     bool is_check_1_loaded = *check_1 == 0x14400021;
    //     bool is_check_2_loaded = *check_2 == 0x14400011;

    //     //printf_syscall("check1: %X, check2: %X\n", *check_1, *check_2);
    //     if (is_check_1_loaded && is_check_2_loaded)
    //     {
    //         *check_1 = 0x0;
    //         *check_2 = 0x0;
    //     }
}

// Basic checks that should run every frame
void MainUpdates(void)
{
    SpeedUpReset();
    FastLoadUpdate();
    ReloadSpyroTimerTick();

    if (gamestate == LOADING_CUTSCENE)
    {
        UpdateHomeworldEntryType();
    }

    if (gamestate == PAUSED)
    {
        UnlockAtlasWarp();
    }

    if (gamestate == GAMEPLAY || gamestate == INTERACTING)
    {
        // lives = 4;

        // Can't do this at runtime because of anti-tamper :)
        //*((u32*)0x80049a50) = 0x0; // NOP the life decrement code during a
        // death. Less annoying than having lives set to 4 over and over, with
        // sfx.

        SaveStartingPositionUpdate();
        ManualSaveSpyroPositionUpdate();


        // Restart the level from the saved position
        if (rawButtonHeld == LOAD_SPYRO_HOTKEY)
        {
            ClearCollectables();
            PrepareSavedSpyroRespawn();
            speedUpResetPending = true;
            if (FastLoadEnabled())
            {
                fastLoadActive = true;
                fastLoadInScenario = false;
                fastLoadFadeMode = FAST_LOAD_CUT_BOTH_FADES;
            }
            RespawnSpyro();
            reloadSpyroTimer = 1; // Used for skipping frozen and fleet npc
                                  // dialogues (see npc_dialogue_skip.c)
        }
        // Restart the level from the spawn point
        else if (rawButtonHeld == RELOAD_LEVEL_HOTKEY || rawButtonHeld == RELOAD_LEVEL_WITH_BALLOON_HOTKEY || rawButtonHeld == RELOAD_LEVEL_CHECKPOINT_HOTKEY) // Theres probably a better way to do this to save space
        {
            u32 reloadLevelType;
            if (rawButtonHeld == RELOAD_LEVEL_HOTKEY)
            {
                reloadLevelType = RELOAD_FULL;
            }
            else if (rawButtonHeld == RELOAD_LEVEL_CHECKPOINT_HOTKEY)
            {
                reloadLevelType = RELOAD_CHECKPOINT;
            }
            else if (rawButtonHeld == RELOAD_LEVEL_WITH_BALLOON_HOTKEY)
            {
                reloadLevelType = RELOAD_BALLOON;
            }

            RestartLevelFromBeginning(reloadLevelType);
        }
        else if (rawButtonHeld == RELOAD_SHEILA_EXIT_HOTKEY)
        {
            speedUpResetPending = true;
            if (FastLoadEnabled())
            {
                fastLoadActive = true;
                fastLoadInScenario = false;
                fastLoadFadeMode = FAST_LOAD_KEEP_FADE_IN;
            }

            SheilaExit();
        }

        else if (rawButtonHeld == RELOAD_SUNNY_EXIT_HOTKEY)
        {
            speedUpResetPending = true;
            if (FastLoadEnabled())
            {
                fastLoadActive = true;
                fastLoadInScenario = false;
                fastLoadFadeMode = FAST_LOAD_KEEP_FADE_IN;
            }

            SunnyExit();
        }
    }

    // if (rawButtonHeld == (L2_BUTTON + R2_BUTTON + CIRCLE_BUTTON))
    // {
    //     FullSaveState();
    // }
    // else if (rawButtonHeld & SELECT_BUTTON)
    // {
    //     FullLoadState();
    // }

    if (HasRecentlyLoadedSpyro() || gamestate == DYING)
    {
        CancelEntryNpcDialogue();
    }


}
