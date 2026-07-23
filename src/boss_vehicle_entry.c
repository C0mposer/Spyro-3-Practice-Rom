#include <types.h>
#include <symbols.h>
#include <gamestates.h>
#include <level_ids.h>
#include <syscalls.h>
#include <common.h>

extern u32 g_ILTimerMode;

// When atlas warping into a boss in IL mode, ensure you enter on a vehicle for proper IL timing
void SetCorrectVehicleInBossUpdate()
{
    if (currentLevel == BUZZS_DUNGEON)
    {
        progressFlags[0x16] = 0x1; // Enable balloon progress flag
        previousLevelIDForVehicleEntry = 40; // Enter on balloon
    }
    else
    {
        progressFlags[0x16] = 0x0; // Disable
    }

    if (currentLevel == SPIKES_ARENA)
    {
        progressFlags[0x17] = 0x1; // Enable whirligig progress flag
    }
    else
    {
        progressFlags[0x17] = 0x0; // Disable
    }

    if (currentLevel == SCORCHS_PIT)
    {
        progressFlags[0x18] = 0x1; // Enable rocket progress flag
    }
    else
    {
        progressFlags[0x18] = 0x0; // Disable
    }

}
