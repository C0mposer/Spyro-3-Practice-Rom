#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include <gamestates.h>

inline static void RespawnSpyro(void)
{
    spyroZ = 0; // Kill spyro by sending him to the void
}

inline static void ClearCollectables(void)
{
    // Clear gui counters
    globalGems = 0;


    // Clear every flag for progress
    memset(&gemsCollectedFlags, 0x0, 0x500);
    memset(&eggsCollectedFlags, 0x0, 0xF8);
    memset(&progressFlags, 0x0, 0x4C);
    memset(skillPointFlags, 0x0, 0x14);

    // Reverse these eventually. These get reset on boot also, so I'm resetting them, assuming they're important.
    memset(0x800716ac, 0x0, 0xf0);
    memset(0x80066eac, 0x0, 0x30);
    memset(0x8007145c, 0x0, 0x28);
    memset(0x80070300, 0x0, 0x28);
}

// Basic checks that should run every frame
void MainUpdates(void)
{
    lives = 99;

    // Restart Level
    if (isButtonHeld == (L1_BUTTON + R1_BUTTON + CIRCLE_BUTTON))
    {
        ClearCollectables();
        RespawnSpyro();
    }
}
