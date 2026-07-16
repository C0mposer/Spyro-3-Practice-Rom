#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include <gamestates.h>
#include <difficulty.h>
#include "menu.h"
#include "menu_elements.h"
#include "menu_triggers.h"
#include <render_helper.h>
#include <timer_options.h>

const char* const menu_toggle_options[2] = { "Off", "On" };

static const char* const difficulty_options[] =
{
    "Easy",
    "Normal",
    "Hard"
};

static const char* const timer_options[] =
{
    "Off",
    "Show Select",
    "Show Always"
};

static const char* const sparx_range_options[] =
{
    "Default",
    "Upgraded"
};


static MenuElement main_menu_elements[] =
{
    MENU_MULTI("Timer", timer_options, TIMER_OFF),
    MENU_MULTI("Difficulty", difficulty_options, DIFFICULTY_MEDIUM),
    MENU_TOGGLE("Fast Reset", false),
    MENU_TOGGLE("Disable Portal", false),
    MENU_TOGGLE("Draw Portals", false),
    MENU_MULTI("Sparx Range", sparx_range_options, 0),
    MENU_TOGGLE("Basket Break", false)
};

Menu main_menu =
{
    .title = "* Practice Menu *",
    .elements = main_menu_elements,
    .x1 = 114,
    .x2 = 400,
    .y1 = 60,
    .y2 = 190,
    .amount_of_elements = (u8)ARRAY_SIZE(main_menu_elements),
    .current_selection = 0,
    .state = MENU_STATE_CLOSED
};

int menu_frames_closed = 0;

static void DrawMenu(Menu* menu)
{
    //DrawScreenColour(1, 0x20, 0, 0x40);
    DrawTextbox(menu->x1, menu->x2, menu->y1, menu->y2);
    DrawTextCentered(menu->title, (menu->x1 + menu->x2) / 2,
        menu->y1 + 7, UNSELECTED_COLOR);
}

static void OpenMenu(Menu* menu)
{
    gamestate = FROZEN;
    PlaySound(14, 0, 0);
    DisableRendering();

    if (lowLevelMusicVolume != 0)
    {
        lowLevelMusicVolume /= 4;
    }

    menu->state = MENU_STATE_OPEN;

    menu_frames_closed = 0;
}

static void CloseMenu(Menu* menu)
{
    gamestate = GAMEPLAY;
    PlaySound(7, 0, 0);

    if (lowLevelMusicVolume != 0)
    {
        lowLevelMusicVolume *= 4;
    }

    menu->state = MENU_STATE_CLOSED;
}

void UpdateMenu(Menu* menu)
{
    if (menu->state == MENU_STATE_CLOSED)
    {
        if (isButtonHeld == L2_BUTTON + R2_BUTTON + TRIANGLE_BUTTON)
        {
            if (gamestate == GAMEPLAY)
            {
                OpenMenu(menu);
            }
        }

        menu_frames_closed++;

        // Must wait a couple frames to re-enable world drawing, so menu fully closes
        if (menu_frames_closed == 2)
        {
            EnableRendering();
        }

        return;
    }

    DrawMenu(menu);
    UpdateMenuElements(menu);

    if (isButtonPressed == CIRCLE_BUTTON || isButtonPressed == TRIANGLE_BUTTON)
    {
        CloseMenu(menu);
    }
}

void UpdateAllMenus(void)
{
    UpdateMenu(&main_menu);
    UpdateMenuTriggers();
}
