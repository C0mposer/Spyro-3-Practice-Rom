#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <types.h>
#include <vector.h>
#include <collision.h>
#include <sparx_upgrades.h>

typedef struct RECT
{
    short x;
    short y;
    short w;
    short h;
} RECT;

extern s32 spyroX;
extern s32 spyroY;
extern s32 spyroZ;
extern s32 spyroAngle;
extern s32 spyroYawAngle;
extern u32 spyroState;
extern u32 spyroHealth;

extern Vec3 respawnPosition; //1.0: 0x8006D0AC
extern u32 respawnAngle;

extern u32 gamestate;
extern u32 framesInScenario;
extern u32 menuState;
extern u32 menuOption;
extern u32 currentMenu;
extern u32 drawScreenBlack; // death/respawn fade level: 0 = clear, 0xFF = full black
extern u32 pauseMusic;

extern Vec3 portalGlideTarget;
extern Vec3 portalGlideDirection;

extern u32 globalGems;
extern u32 globalEggs;
extern u32 lives;

extern u32 currentLevel;

extern u32 levelID;
extern u32 subLevelID;

extern u32 levelIndex;
extern u32 isHomeworldLevel;

// Source level remembered by the balloon entry code
extern u32 previousLevelIDForVehicleEntry;

extern u32 previousLevelID; // Same as above, just simpler name to use for other purposes

extern u32 gemsCollectedPerLevel[40];
extern u8 gemsCollectedFlags[0x500];
extern u8 eggsCollectedBitmask[40];
extern u8 eggsCollectedSinceEnteredBitmask;
extern u8 eggDifficultyState[0xF0];
extern u8 recordedPerformancePerEgg[0xF0];
extern u8 progressFlags[0x4C];
extern u8 dialogueFlags[40];
extern u8 skillPointFlags[0x14];
extern u8 hasEnteredLevelFlags[40];

extern u8 currentCheckpointData[0x850];
extern u8 savedCheckpointData[0x850];
extern u32 savedCheckpointUpdated;
extern u32 savedCheckpointSwimState;
extern u32 checkpointCameraMode;

extern u8 sparxState[0x28];
extern u8 sparxDefaults[0x28];

// Adaptive difficulty state. globalDifficulty is the legacy name for
// adaptiveDifficultyScore and is kept for existing code.
extern s32 adaptiveDifficultyScore;
extern s32 globalDifficulty;
extern s32 baseDifficulty;
extern s32 currentDifficulty;
extern u32 totalHits;

// Per-boss death counters used by the game's adaptive boss logic.
extern u8 deathsToBuzz;
extern u8 deathsToSpike;
extern u8 deathsToScorch;
extern u8 deathsToSorceress;

extern u32 upgradeFlags;

extern u32 isButtonHeld;
extern u32 isButtonPressed;
extern u32 rawButtonHeld;
extern u32 rawButtonPressed;
extern u32 rawButtonReleased;

extern bool analogEnabled;
extern u8 leftAnalogX;
extern u8 leftAnalogY;

extern u32 optionsMusicVolumeSlider;
extern s16 lowLevelMusicVolume;

extern u8 titleScreenState;
extern u8 titleScreenPreviousState;
extern u8 titleScreenSubstate;
extern s32 globalTimer;
extern s32* sunriseUpdateParticlePtr;

extern int currentMusicSector;
extern int musicEndSector;
extern int musicInterleavedTrack;

extern u32 levelTimer; // 60fps

extern s32 difficultyOverride;

extern u32 eggsCollectedThisVisit;
extern u32 eggsCollectedSinceLevelEntry; // Research the difference between this, and the symbol above more. They seem related, but not identical.

extern CollisionHeader* TerrainCollisionData;
extern TriggerRecord** ptr_levelRelated;
extern u32* ptr_primitiveStruct;
extern u32* primitiveBufferEnd;

extern SparxRangeData game_sparx_range_data;

extern u32 fade_effect;

extern u32 buzz_health;
extern u32 spike_health;
extern u32 scorch_health;
extern u32 sorc_health;
extern u32 final_sorc_health;


int sprintf(char* buffer, char* fmt, ...);
void* memset(void* dest, u32 value, u32 size);
void MemCopy(void* destination, const void* source, u32 size);

void DrawTextboxSquare(s16 x1, s16 x2, s16 y1, s16 y2);
extern void DrawTextbox(s32 x1, s32 x2, s32 y1, s32 y2);
extern void DrawShadedLine(s16 x1, s16 y1, s16 x2, s16 y2);
void DrawTextCentered(const char* text, s32 x, s32 y, s32 color);
void DrawScreenColour(s32 unk, u8 R, u8 G, u8 B);

// \* is star > is square
extern void DrawText(const char* text, s32 x, s16 y, s32 colour, s32* unkptr);

s32* DropGem(s32 object, uint param_2, s32 vec1, s32 vec2);
s32 PlaySound(s32 soundID, s32 param_2, u8 param_3);

void SetPrimitiveDrawMode(s32 param_1);

void LoadLevel(s32 splashScreenIndex, s32 levelID);

void test();
char* SunriseUpdateParticles(s32 param_1, s32 param_2, Vec3* param_3, Vec3* param_4);

void Vec3Copy(Vec3* out, Vec3* in);

void LoadImage(RECT* r, u8* texture_in_mainram);

void SetAudioData(int startSector, int endSector, int interleavedTrack);

void SpawnMoby(int id, Vec3* position);

void BeginLevelLoad(s32 context);
void LoadLevelRelated(void);
void UpdateDuringLoadingGlide(void);
s32 MoveSpyroTowardPoint(Vec3* target);
void UpdateSpyroAnimation(void);

void DrawPortals(void);
void UnpackCollisionTriangle(s32 triangle_index, Triangle* triangle);
void DrawPortalRelated(Vec3* screen_position, const Vec3* world_position, u32 shift);
u8 CalcScreenClipFlags(s32 x, s32 y, s32 depth);
void AddToWorldTable(void* primitive, s32 depth_bucket);
void DrawSync(int);

void ReadCdSync(int baseSector, unsigned int* buffer, int numBytes, int offsetByte); // 0x80050578
void FlushCache(void); // 0x800649BC

int ReadCdAsync(int baseSector, unsigned int* buffer, int numBytes, int offsetBytes); // 0x80050680

bool WaitForCd_(void); // 0x800503F8

void DialogueUpdate(void);

void SaveCheckpointMemoryPatch(u8* targetAddress, u8 valueSize, u8 *checkpoint); // valueSize is actually an enum for 0 = 1byte, 1 = 2byte, 2 = 4byte

#endif //SYMBOLS_H
