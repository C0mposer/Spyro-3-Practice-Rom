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
    MENU_MULTI(MAIN_MENU_PAGE_GENERAL, "IL Timer", timer_options, TIMER_OFF),
    MENU_MULTI(MAIN_MENU_PAGE_GENERAL, "Manual Timer", timer_options, TIMER_OFF),
    MENU_MULTI(MAIN_MENU_PAGE_GENERAL, "Difficulty", difficulty_options,
               DIFFICULTY_MEDIUM),
    MENU_TOGGLE(MAIN_MENU_PAGE_GENERAL, "Disable Portals", false),
    MENU_TOGGLE(MAIN_MENU_PAGE_GENERAL, "Draw Portals", false),
    MENU_TOGGLE(MAIN_MENU_PAGE_GENERAL, "Zombie Mode", false),
    MENU_TOGGLE(MAIN_MENU_PAGE_QOL, "Fast Reset", true),
    MENU_TOGGLE(MAIN_MENU_PAGE_QOL, "Mash Dialogue", false),
    MENU_TOGGLE(MAIN_MENU_PAGE_QOL, "Disable Bianca", false),
    MENU_TOGGLE(MAIN_MENU_PAGE_SPECIFIC, "Rhynoc Trainer", false),
    MENU_MULTI(MAIN_MENU_PAGE_SPECIFIC, "Sparx Range", sparx_range_options, 0),
    MENU_TOGGLE(MAIN_MENU_PAGE_SPECIFIC, "Basket Break", false),
    MENU_SET_VALUE(MAIN_MENU_PAGE_SPECIFIC, "Gem Count", 0, 20000,
                   &globalGems, 0),
    MENU_SET_VALUE(MAIN_MENU_PAGE_SPECIFIC, "Egg Count", 0, 100,
                   &globalEggs, 0)};

Menu main_menu = {.page_titles = main_menu_page_titles,
                  .elements = main_menu_elements,
                  .x1 = 110,
                  .x2 = 400,
                  .y1 = 20,
                  .y2 = 227,
                  .amount_of_elements = (u8)ARRAY_SIZE(main_menu_elements),
                  .current_selection = 0,
                  .state = MENU_STATE_CLOSED,
                  .amount_of_pages = MAIN_MENU_PAGE_COUNT,
                  .current_page = MAIN_MENU_PAGE_GENERAL};

int menu_frames_closed = 0;
