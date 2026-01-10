#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include <gamestates.h>
#include <upgrades.h>

Vec3 savedStartingPosition = { 0 };
Vec3 savedStartingPositionSubLevel = { 0 };
u32 savedStartingAngle = 0;
u32 savedStartingAngleSubLevel = 0;

inline static void SaveStartingPositionUpdate(void)
{
    static u32 currentLevelID = 0;
    static u32 currentSubLevelID = 0;

    if (gamestate == GAMEPLAY)
    {
        if (currentLevelID != levelID)
        {

            Vec3Copy(&savedStartingPosition, &respawnPosition); // Save the respawn position for the current level/sub level
            savedStartingAngle = respawnAngle; // Save the respawn position for the current level/sub level

            currentLevelID = levelID;

        }
        if (currentSubLevelID != subLevelID)
        {
            Vec3Copy(&savedStartingPositionSubLevel, &respawnPosition); // Save the respawn position for the current level/sub level
            savedStartingAngleSubLevel = respawnAngle; // Save the respawn position for the current level/sub level

            currentSubLevelID = subLevelID;

        }
    }
}

inline static void ReloadLevelStartingPosition(void)
{
    if (subLevelID == 0) {
        Vec3Copy(&respawnPosition, &savedStartingPosition);
        respawnAngle = savedStartingAngle;
    }
    else
    {
        Vec3Copy(&respawnPosition, &savedStartingPositionSubLevel);
        respawnAngle = savedStartingAngleSubLevel;
    }
}

inline static void RespawnSpyro(void)
{
    spyroZ = 0; // Kill spyro by sending him to the void
}

inline static void ClearCollectables(void)
{
    // Clear gui counters
    globalGems = 0;
    globalEggs = 0;

    // Clear every flag for progress
    memset(&gemsCollectedFlags, 0x0, 0x500);
    memset(&eggsCollectedFlags, 0x0, 0xF8);
    memset(&progressFlags, 0x0, 0x4C);
    memset(skillPointFlags, 0x0, 0x14);

    // Reverse these eventually. These get reset on boot also, so I'm resetting them, assuming they're important.
    memset((void*)0x800716ac, 0x0, 0xf0);
    memset((void*)0x80066eac, 0x0, 0x30);
    memset((void*)0x8007145c, 0x0, 0x28);
    memset((void*)0x80070300, 0x0, 0x28);
}

inline static void UnlockAtlasWarp()
{
    upgradeFlags |= ATLAS_WARP; // Always set the atlas warp flag to true
    memset(hasEnteredLevelFlags, 0x11111111, 40); // Unlock all levels in atlas
}

// Basic checks that should run every frame
void MainUpdates(void)
{
    if (gamestate == GAMEPLAY)
    {
        lives = 99;

        UnlockAtlasWarp();

        SaveStartingPositionUpdate(); // Check if the starting position needs to be saved. If so, save it.

        // Restart Level
        if (isButtonHeld == (L1_BUTTON + R1_BUTTON + CIRCLE_BUTTON))
        {
            ClearCollectables();
            ReloadLevelStartingPosition();
            RespawnSpyro();
        }
    }
}
