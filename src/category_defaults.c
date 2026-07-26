#include <types.h>
#include <symbols.h>
#include <level_ids.h>
#include "menu/menu.h"
#include <difficulty.h>
#include <category_defaults.h>

void UnlockAllSideCharacterPortals()
{
    eggsCollectedBitmask[0x6] = 0b1; // Set sheila egg as collected
    eggsCollectedBitmask[0xF] = 0b1; // Set sgt byrd egg as collected
    eggsCollectedBitmask[0x18] = 0b1; // Set bently egg as collected
    eggsCollectedBitmask[0x21] = 0b100; // Set agent 9 egg as collected
}

void CloseAllSideCharacterPortals()
{
    eggsCollectedBitmask[0x6] = 0b0; // Set sheila egg as uncollected
    eggsCollectedBitmask[0xF] = 0b0; // Set sgt byrd egg as uncollected
    eggsCollectedBitmask[0x18] = 0b0; // Set bently egg as uncollected
    eggsCollectedBitmask[0x21] = 0b0; // Set agent 9 egg as uncollected
}

// Used to set certain default settings based on what category is chosen
void CategoryDefaultsUpdate()
{
    // Lock/Unlock menu elements
    if (main_menu.elements[CATEGORY_MULTI].selection_option == MANUAL)
    {
        main_menu.elements[DIFFICULTY_MULTI].disabled = false;
        main_menu.elements[SPARX_RANGE_TOGGLE].disabled = false;
        main_menu.elements[BASKET_BREAK_TOGGLE].disabled = false;
        main_menu.elements[ZOMBIE_MODE_TOGGLE].disabled = false;
    }
    else
    {
        main_menu.elements[DIFFICULTY_MULTI].disabled = true;
        main_menu.elements[SPARX_RANGE_TOGGLE].disabled = true;
        main_menu.elements[BASKET_BREAK_TOGGLE].disabled = true;
        main_menu.elements[ZOMBIE_MODE_TOGGLE].disabled = true;
    }

    // Logic for different category defaults
    if (main_menu.elements[CATEGORY_MULTI].selection_option == ANY)
    {
        CloseAllSideCharacterPortals();

        if (currentLevel == CHARMED_RIDGE)
        {
            main_menu.elements[ZOMBIE_MODE_TOGGLE].enabled = true; // Enable zombie for charmed
        }
        else
        {
            main_menu.elements[ZOMBIE_MODE_TOGGLE].enabled = false; // Disable zombie
        }

        if (currentLevel < 29)
        {
            main_menu.elements[DIFFICULTY_MULTI].selection_option = DIFFICULTY_MEDIUM; // Set difficulty for all of sunrise and midday
        }
        else
        {
            main_menu.elements[DIFFICULTY_MULTI].selection_option = DIFFICULTY_HARD; // Set difficulty for all of evening and midnight
        }

        // No Sparx Upgrades
        main_menu.elements[SPARX_RANGE_TOGGLE].enabled = false;
        main_menu.elements[BASKET_BREAK_TOGGLE].enabled = false;
    }
    else if (main_menu.elements[CATEGORY_MULTI].selection_option == HUNDRED_EGG)
    {
        UnlockAllSideCharacterPortals();

        main_menu.elements[ZOMBIE_MODE_TOGGLE].enabled = false; // Disable zombie when switching categories

        if (currentLevel <= 11)
        {
            main_menu.elements[DIFFICULTY_MULTI].selection_option = DIFFICULTY_MEDIUM; // Set medium for sunny
        }
        else
        {
            main_menu.elements[DIFFICULTY_MULTI].selection_option = DIFFICULTY_HARD; // Set hard for everything else
        }

        // No Sparx Upgrades
        main_menu.elements[SPARX_RANGE_TOGGLE].enabled = false;
        main_menu.elements[BASKET_BREAK_TOGGLE].enabled = false;
    }
    else if (main_menu.elements[CATEGORY_MULTI].selection_option == ONE_SEVENTEEN)
    {
        UnlockAllSideCharacterPortals();

        main_menu.elements[ZOMBIE_MODE_TOGGLE].enabled = false; // Disable zombie when switching categories

        if (currentLevel <= 11)
        {
            main_menu.elements[DIFFICULTY_MULTI].selection_option = DIFFICULTY_MEDIUM; // Set medium for sunny
        }
        else
        {
            main_menu.elements[DIFFICULTY_MULTI].selection_option = DIFFICULTY_HARD; // Set hard for everything else
        }

        // Sparx upgrade defaults
        if (currentLevel >= 20)
        {
            main_menu.elements[SPARX_RANGE_TOGGLE].enabled = true;
        }
        else
        {
            main_menu.elements[SPARX_RANGE_TOGGLE].enabled = false;
        }

        if (currentLevel >= 40)
        {
            main_menu.elements[BASKET_BREAK_TOGGLE].enabled = true;
        }
        else
        {
            main_menu.elements[BASKET_BREAK_TOGGLE].enabled = false;
        }
    }
}
