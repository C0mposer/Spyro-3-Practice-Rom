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

const char* const difficulty_options[] =
{
    "Easy",
    "Normal",
    "Hard"
};

const char* const timer_options[] =
{
    "Off",
    "Show Stopped",
    "Show Always"
};

const char* const sparx_range_options[] =
{
    "Default",
    "Upgraded"
};


extern MenuElement main_menu_elements[];

extern Menu main_menu;

extern int menu_frames_closed;

static void DrawMenu(Menu* menu)
{
    //DrawScreenColour(1, 0x20, 0, 0x40);
    DrawTextbox(menu->x1, menu->x2, menu->y1, menu->y2);
    DrawTextCentered(menu->page_titles[menu->current_page],
        (menu->x1 + menu->x2) / 2,
        menu->y1 + 7, UNSELECTED_COLOR);
    DrawMenuPageControls(menu);
}

static void OpenMenu(Menu* menu)
{
    gamestate = FROZEN;
    PlaySound(14, 0, 0);
    DisableRendering();

    // if (lowLevelMusicVolume != 0)
    // {
    //     lowLevelMusicVolume /= 4;
    // }

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

    UpdateMenuPage(menu);
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
