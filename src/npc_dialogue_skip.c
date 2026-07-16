#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include <buttons.h>
#include <gamestates.h>
#include <upgrades.h>
#include <hotkeys.h>
#include "menu/menu.h"
#include <level_ids.h>

extern u32 reloadSpyroTimer;

// When reloading a saved Spyro position, set the beginning of level NPC's to inactive so they won't pull you towards them
void CancelEntryNpcDialogue()
{
    if (currentLevel == FROZEN_ALTARS)
    {
        #define state_offset 0x48
        raw_memory_ptr* gabrielle_moby = 0x80191244;

        gabrielle_moby[state_offset] = 0xFD; // Set gabrielle to not talk to us again
    }
    else if (currentLevel == LOST_FLEET)
    {
        #define state_offset 0x48
        raw_memory_ptr* gabrielle_moby = 0x80191990;

        gabrielle_moby[state_offset] = 0xFD; // Set Crazy Ed to not talk to us again
    }
}

// Tick a timer up for 40 frames after loading a saved spyro position
void ReloadSpyroTimerTick()
{
    if (reloadSpyroTimer > 40)
    {
        reloadSpyroTimer = 0;
    }
    else if (reloadSpyroTimer > 0)
    {
        reloadSpyroTimer++;
    }
}

// If it's been 40 frames or less since you've loaded a saved spyro position.
bool HasRecentlyLoadedSpyro()
{
    //printf_syscall("reloadSpyroTimer = %d\n", reloadSpyroTimer);
    if (reloadSpyroTimer > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}