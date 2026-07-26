#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include <upgrades.h>
#include <difficulty.h>
#include <gamestates.h>
#include "menu.h"
#include "menu_triggers.h"
#include <bianca_flags.h>
#include <savestate_buttons.h>
#include <buttons.h>


static void DifficultyTrigger()
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
extern bool g_shouldDisablePortal;
static void DisablePortalTrigger()
{

    g_shouldDisablePortal = main_menu.elements[DISABLE_PORTAL_TOGGLE].enabled;
}

extern int g_ILTimerMode;
static void ILTimerTrigger()
{
    g_ILTimerMode = main_menu.elements[IL_TIMER_MULTI].selection_option;
    //DrawText("Updating", 20, 20, 1, 0);
}

extern int g_manualTimerMode;
static void TimerTrigger()
{
    g_manualTimerMode = main_menu.elements[TIMER_MULTI].selection_option;
    //DrawText("Updating", 20, 20, 1, 0);
}

const SparxRangeData sparx_range_default = { 0x80E, 0x15E, 0x280 };
const SparxRangeData sparx_range_upgraded = { 0xC00, 0x3C0, 0x20D };
static void SparxRangeTrigger()
{
    if (!main_menu.elements[SPARX_RANGE_TOGGLE].enabled)
    {
        game_sparx_range_data = sparx_range_default;
        //printf_syscall("Default\n");
    }
    else
    {
        game_sparx_range_data = sparx_range_upgraded;
        //printf_syscall("Upgraded\n");
    }
}

static void BasketBreakTrigger()
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

static void DisableBiancaTrigger()
{
    if (main_menu.elements[DISABLE_BIANCA_TOGGLE].enabled)
    {
        progressFlags[BIANCA_SUNRISE_FLAG_OFFSET] = true;
        progressFlags[BIANCA_MIDDAY_FLAG_OFFSET] = true;
        progressFlags[BIANCA_EVENING_FLAG_OFFSET] = true;
        progressFlags[BIANCA_MIDNIGHT_FLAG_OFFSET] = true;
    }
}

extern bool g_rhynocTrainerEnabled;
static void RhynocTrainerTrigger()
{
    if (!main_menu.elements[RHYNOC_TRAINER_TOGGLE].enabled)
    {
        g_rhynocTrainerEnabled = false;
    }
    else
    {
        g_rhynocTrainerEnabled = true;
    }
}

extern bool g_fastDialogueEnabled;
static void FastDialougeTrigger()
{
    if (!main_menu.elements[FAST_DIALOUGE_TRIGGER].enabled)
    {
        g_fastDialogueEnabled = false;
    }
    else
    {
        g_fastDialogueEnabled = true;
    }
}

extern bool shouldDisableZombieOnce;
extern bool shouldEnableZombieOnce;
static void ZombieModeToggle()
{
    if (!main_menu.elements[ZOMBIE_MODE_TOGGLE].enabled)
    {
        if (shouldDisableZombieOnce == true)
        {
            spyroHealth = 3;

            shouldEnableZombieOnce = true;
            shouldDisableZombieOnce = false;
        }
    }
    else
    {
        if (shouldEnableZombieOnce == true)
        {
            spyroHealth = -1;

            shouldEnableZombieOnce = false;
            shouldDisableZombieOnce = true;
        }
    }
}

extern u32 savestate_button_option;
static void SavestateButtonToggle()
{
    if (main_menu.elements[SAVESTATE_BUTTON_MULTI].selection_option == SAVESTATE_L3)
    {
        savestate_button_option = L3_BUTTON;
    }
    else if (main_menu.elements[SAVESTATE_BUTTON_MULTI].selection_option == SAVESTATE_L1_L2)
    {
        savestate_button_option = L1_BUTTON | L2_BUTTON;
    }
    else if (main_menu.elements[SAVESTATE_BUTTON_MULTI].selection_option == SAVESTATE_L1_R2)
    {
        savestate_button_option = L1_BUTTON | R2_BUTTON;
    }
}

extern u32 loadstate_button_option;
static void LoadstateButtonToggle()
{
    if (main_menu.elements[LOADSTATE_BUTTON_MULTI].selection_option == LOADSTATE_R3)
    {
        loadstate_button_option = R3_BUTTON;
    }
    else if (main_menu.elements[LOADSTATE_BUTTON_MULTI].selection_option == LOADSTATE_SELECT)
    {
        loadstate_button_option = SELECT_BUTTON;
    }
    else if (main_menu.elements[LOADSTATE_BUTTON_MULTI].selection_option == LOADSTATE_R1_R2)
    {
        loadstate_button_option = R1_BUTTON | R2_BUTTON;
    }
    else if (main_menu.elements[LOADSTATE_BUTTON_MULTI].selection_option == LOADSTATE_L2_R1)
    {
        loadstate_button_option = L2_BUTTON | R1_BUTTON;
    }
}

static void SparxHealthToggle()
{
    if (main_menu.elements[SET_HEALTH_TOGGLE].selection_option == 1)
    {
        spyroHealth = 3;
    }
    else if (main_menu.elements[SET_HEALTH_TOGGLE].selection_option == 2)
    {
        spyroHealth = 2;
    }
    else if (main_menu.elements[SET_HEALTH_TOGGLE].selection_option == 3)
    {
        spyroHealth = 1;
    }
    else if (main_menu.elements[SET_HEALTH_TOGGLE].selection_option == 4)
    {
        spyroHealth = 0;
    }
    else
    {
        return;
    }
}


// Update the Off/On logic
void UpdateMenuTriggers()
{
    DifficultyTrigger();
    DisablePortalTrigger();
    TimerTrigger();
    ILTimerTrigger();
    SparxRangeTrigger();
    BasketBreakTrigger();
    DisableBiancaTrigger();
    RhynocTrainerTrigger();
    FastDialougeTrigger();
    ZombieModeToggle();
    SavestateButtonToggle();
    LoadstateButtonToggle();
    SparxHealthToggle();
}
