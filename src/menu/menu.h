#ifndef MENU_H
#define MENU_H

#include <types.h>

#define UNSELECTED_COLOR 0x4
#define SELECTED_COLOR 0x6

typedef enum MenuType {
    MENU_TYPE_TOGGLE,
    MENU_TYPE_MULTI,
    MENU_TYPE_VALUE_SET
} MenuType;

typedef enum MenuState { MENU_STATE_CLOSED, MENU_STATE_OPEN } MenuState;

enum MainMenuPage
{
    MAIN_MENU_PAGE_GENERAL,
    MAIN_MENU_PAGE_SAVESTATE,
    MAIN_MENU_PAGE_QOL,
    MAIN_MENU_PAGE_SPECIFIC,
    MAIN_MENU_PAGE_COUNT
};

enum MainMenuElement {
    CATEGORY_MULTI,
    IL_TIMER_MULTI,
    TIMER_MULTI,
    DIFFICULTY_MULTI,
    DISABLE_PORTAL_TOGGLE,
    DRAW_PORTAL_POLYGONS_TOGGLE,
    ZOMBIE_MODE_TOGGLE,
    FAST_LOAD_TOGGLE,
    FAST_DIALOUGE_TRIGGER,
    DISABLE_BIANCA_TOGGLE,
    RHYNOC_TRAINER_TOGGLE,
    SPARX_RANGE_TOGGLE,
    BASKET_BREAK_TOGGLE,
    SET_GEM_COUNT_TOGGLE,
    SET_EGG_COUNT_TOGGLE,
    MAIN_MENU_ELEMENT_COUNT
};

typedef struct MenuElement
{
    const char* label;
    const char* const* options;

    union
    {
        bool enabled;
        u32 selection_option;
        u32 value;
    };
    u32* symbol_to_set;

    u8 option_count;
    u8 type;
    u8 page;
} MenuElement;

typedef struct Menu
{
    const char* const* page_titles;
    MenuElement* elements;

    s16 x1;
    s16 x2;
    s16 y1;
    s16 y2;

    u8 amount_of_elements;
    u8 current_selection;
    u8 state;
    u8 amount_of_pages;
    u8 current_page;
} Menu;

extern const char* const menu_toggle_options[2];
extern const char* const main_menu_page_titles[MAIN_MENU_PAGE_COUNT];

#define MENU_TOGGLE(page_, label_, initial_)                                   \
    {.label = (label_),                                                        \
     .options = menu_toggle_options,                                           \
     .selection_option = (initial_),                                           \
     .option_count = 2,                                                        \
     .type = MENU_TYPE_TOGGLE,                                                 \
     .page = (page_)}

/* must be an array so its option count can be calculated here. */
#define MENU_MULTI(page_, label_, choices_, initial_)                          \
    {.label = (label_),                                                        \
     .options = (choices_),                                                    \
     .selection_option = (initial_),                                           \
     .option_count = (u8)ARRAY_SIZE(choices_),                                 \
     .type = MENU_TYPE_MULTI,                                                  \
     .page = (page_)}

#define MENU_SET_VALUE(page_, label_, min_, max_, symbol_to_set_, initial_)    \
    {.label = (label_),                                                        \
     .value = (initial_),                                                      \
     .option_count = sizeof(u16),                                              \
     .type = MENU_TYPE_VALUE_SET,                                              \
     .symbol_to_set = symbol_to_set_,                                          \
     .page = (page_)}

u8 GetMenuPageElementCount(const Menu* menu);
MenuElement* GetMenuPageElement(Menu* menu, u8 page_index);
void UpdateMenuPage(Menu* menu);
void DrawMenuPageControls(const Menu* menu);

void UpdateMenu(Menu* menu);
void UpdateAllMenus(void);

extern Menu main_menu;

#endif /* MENU_H */
