#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include <syscalls.h>
#include <gamestates.h>
#include "menu.h"
#include "menu_elements.h"
#include "menu_triggers.h"

// Menu Data
Menu main_menu = { .title = "* Practice Menu *",
                   .x1 = 114,
                   .x2 = 400,
                   .y1 = 60,
                   .y2 = 180,
                   .amount_of_elements = 0,
                   .current_selection = 0, };

MenuElement vibration_toggle = { .type = MENU_TYPE_TOGGLE,
                                .enabled = false,
                                .text[0] = "Vibration Off",
                                .text[1] = "Vibration On" };

MenuElement balls_toggle = { .type = MENU_TYPE_TOGGLE,
                             .enabled = false,
                             .text[0] = "Test 2 Off",
                             .text[1] = "Test 2 On" };


MenuElement cheat_toggle = { .type = MENU_TYPE_TOGGLE,
                             .enabled = false,
                             .text[0] = "Test 3 Off",
                             .text[1] = "Test 3 On" };


MenuElement test_toggle = { .type = MENU_TYPE_TOGGLE,
                            .enabled = false,
                            .text[0] = "Test 4 Off",
                            .text[1] = "Test 4 On" };


MenuElement test5_toggle = { .type = MENU_TYPE_TOGGLE,
                             .enabled = false,
                             .text[0] = "Test 5 Off",
                             .text[1] = "Test 5 On" };


// Private Logic
static void DrawMenu(Menu* menu)
{
    DrawScreenColour(1, 0x20, 0, 0x40);
    DrawTextbox(menu->x1, menu->x2, menu->y1, menu->y2);

    s32 line_x1 = menu->x1 + 10;
    s32 line_y1 = menu->y1 + 20;
    s32 line_x2 = menu->x2 - 10;
    DrawShadedLine(line_x1, line_y1, line_x2, line_y1);

    s32 line_center = (line_x1 + line_x2) / 2;
    DrawTextCentered(menu->title, line_center, line_y1 - 13, 0x4);
}

static void MenuStateClosed(Menu* menu)
{
    // Check for button combo to start opening menu
    if (isButtonHeld == L2_BUTTON + R2_BUTTON + TRIANGLE_BUTTON)
    {
        menu->state = MENU_STATE_OPENING;
    }
}

static void MenuStateOpening(Menu* menu)
{
    // Freeze the game, turn down music, and play a sound before opening the menu
    gamestate = FROZEN;
    PlaySound(14, 0, 0);

    if (lowLevelMusicVolume != 0)
    {
        lowLevelMusicVolume /= 4;
    }

    menu->state = MENU_STATE_OPEN;
}

static void MenuStateOpen(Menu* menu)
{
    DrawMenu(menu);
    UpdateMenuElements(menu);

    // Check for button to start closing the menu
    if (isButtonPressed == CIRCLE_BUTTON || isButtonPressed == TRIANGLE_BUTTON)
    {
        menu->state = MENU_STATE_CLOSING;
    }
}

static void MenuStateClosing(Menu* menu)
{
    // Unfreeze the game, play a sound, and turn music back up 
    gamestate = GAMEPLAY;
    PlaySound(7, 0, 0);

    if (lowLevelMusicVolume != 0)
    {
        lowLevelMusicVolume *= 4;
    }

    menu->state = MENU_STATE_CLOSED;
}

// Public API
void UpdateMenu(Menu* menu)
{
    switch (menu->state)
    {
        case MENU_STATE_CLOSED:
        {
            MenuStateClosed(menu);
            break;
        }
        case MENU_STATE_OPENING:
        {
            MenuStateOpening(menu);
            break;
        }
        case MENU_STATE_OPEN:
        {
            MenuStateOpen(menu);
            break;
        }
        case MENU_STATE_CLOSING:
        {
            MenuStateClosing(menu);
            break;
        }
    }
}

void UpdateAllMenus()
{
    ONCE
    {
        AddMenuElement(&main_menu, vibration_toggle);
        AddMenuElement(&main_menu, balls_toggle);
        AddMenuElement(&main_menu, cheat_toggle);
        AddMenuElement(&main_menu, test_toggle);
        AddMenuElement(&main_menu, test5_toggle);
    }

    UpdateMenu(&main_menu);
    UpdateMenuTriggers();
}
