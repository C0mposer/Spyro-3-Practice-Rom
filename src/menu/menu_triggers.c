#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include <upgrades.h>
#include <difficulty.h>
#include <gamestates.h>
#include "menu.h"
#include "menu_triggers.h"

// Update the Off/On logic
void UpdateMenuTriggers()
{
    DifficultyTrigger();
    DisablePortalTrigger();
    TimerTrigger();
    SparxRangeTrigger();
    BasketBreakTrigger();
}

void DifficultyTrigger()
{
    if (main_menu.elements[DIFFICULTY_MULTI].selection_option == DIFFICULTY_EASY)
    {
        SetDifficulty(DIFFICULTY_EASY);
    }
    else if (main_menu.elements[DIFFICULTY_MULTI].selection_option == DIFFICULTY_MEDIUM)
    {
        SetDifficulty(DIFFICULTY_MEDIUM);
    }
    else
    {
        SetDifficulty(DIFFICULTY_HARD);
    }
}

// Simply creating a global to access the menu off/on state
bool g_shouldDisablePortal;
void DisablePortalTrigger()
{

    g_shouldDisablePortal = main_menu.elements[DISABLE_PORTAL_TOGGLE].enabled;
}

int g_manualTimerMode;
void TimerTrigger()
{
    g_manualTimerMode = main_menu.elements[TIMER_MULTI].selection_option;
    //DrawText("Updating", 20, 20, 1, 0);
}

SparxRangeData sparx_range_default = { 0x80E, 0x15E, 0x280 };
SparxRangeData sparx_range_upgraded = { 0xC00, 0x3C0, 0x20D };
void SparxRangeTrigger()
{
    if (!main_menu.elements[SPARX_RANGE_TOGGLE].enabled)
    {
        game_sparx_range_data = sparx_range_default;
        printf_syscall("Default\n");
    }
    else
    {
        game_sparx_range_data = sparx_range_upgraded;
        printf_syscall("Upgraded\n");
    }
}

void BasketBreakTrigger()
{
    if (!main_menu.elements[BASKET_BREAK_TOGGLE].enabled)
    {
        upgradeFlags = UPGRADE_FLAGS_NONE;
    }
    else
    {
        upgradeFlags = BREAK_BASKETS_FLAG;
    }
}