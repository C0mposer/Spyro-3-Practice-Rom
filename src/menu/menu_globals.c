#include "menu.h"
#include "menu_elements.h"
#include "menu_triggers.h"
#include <buttons.h>
#include <difficulty.h>
#include <gamestates.h>
#include <render_helper.h>
#include <symbols.h>
#include <timer_options.h>
#include <types.h>

// Put here because globals that change cant be in executable ram because of
// anti piracy. The explicit bounds keep these arrays complete so MENU_MULTI can
// use ARRAY_SIZE.
extern const char* const difficulty_options[3];
extern const char* const timer_options[3];
extern const char* const sparx_range_options[2];

MenuElement main_menu_elements[] = {
    MENU_MULTI("Timer", timer_options, TIMER_OFF),
    MENU_MULTI("Difficulty", difficulty_options, DIFFICULTY_MEDIUM),
    MENU_TOGGLE("Disable Portals", false),
    MENU_MULTI("Sparx Range", sparx_range_options, 0),
    MENU_TOGGLE("Basket Break", false),
    MENU_TOGGLE("Draw Portals", false),
    MENU_TOGGLE("Fast Reset", false),
    MENU_TOGGLE("Fast Dialouge", false),
    MENU_TOGGLE("Disable Bianca", false),
    MENU_TOGGLE("Rhynoc Trainer", false),
    MENU_SET_VALUE("Gem Count", 0, 20000, &globalGems, 0),
    MENU_SET_VALUE("Egg Count", 0, 100, &globalEggs, 0)};

Menu main_menu = {.title = "* Practice Menu *",
                  .elements = main_menu_elements,
                  .x1 = 110,
                  .x2 = 400,
                  .y1 = 20,
                  .y2 = 220,
                  .amount_of_elements = (u8)ARRAY_SIZE(main_menu_elements),
                  .current_selection = 0,
                  .state = MENU_STATE_CLOSED};

int menu_frames_closed = 0;
