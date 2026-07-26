#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include <buttons.h>
#include <gamestates.h>
#include <upgrades.h>
#include <hotkeys.h>
#include "menu/menu.h"
#include <level_ids.h>

extern bool shouldDisableEntryNpc;
extern u8 entryNpcLevel;

void RememberEntryNpcDialogueSkip()
{
    if (currentLevel == FROZEN_ALTARS || currentLevel == LOST_FLEET)
    {
        shouldDisableEntryNpc = true;
        entryNpcLevel = currentLevel;
    }
}

void ClearEntryNpcDialogueSkip()
{
    shouldDisableEntryNpc = false;
}

// When reloading a saved Spyro position, set the beginning of level NPC's to inactive so they won't pull you towards them
void CancelEntryNpcDialogue()
{
    if (!shouldDisableEntryNpc)
        return;

    if (currentLevel != entryNpcLevel)
    {
        shouldDisableEntryNpc = false;
        return;
    }

    if (subLevelID != 0)
        return;

    u8* entryNpc;

    if (currentLevel == FROZEN_ALTARS)
        entryNpc = (u8*)0x80191244; // Gabrielle
    else
        entryNpc = (u8*)0x80191990; // Crazy Ed

    entryNpc[0x48] = 0xFD; // Mark them as inactive so they don't drag you into their talk box
}
