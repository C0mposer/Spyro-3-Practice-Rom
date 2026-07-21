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

enum MainMenuElement {
    IL_TIMER_MULTI,
    TIMER_MULTI,
    DIFFICULTY_MULTI,
    DISABLE_PORTAL_TOGGLE,
    DRAW_PORTAL_POLYGONS_TOGGLE,
    FAST_LOAD_TOGGLE,
    FAST_DIALOUGE_TRIGGER,
    DISABLE_BIANCA_TOGGLE,
    RHYNOC_TRAINER_TOGGLE,
    SPARX_RANGE_TOGGLE,
    BASKET_BREAK_TOGGLE,
    ZOMBIE_MODE_TOGGLE,
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
} MenuElement;

typedef struct Menu
{
    const char* title;
    MenuElement* elements;

    s16 x1;
    s16 x2;
    s16 y1;
    s16 y2;

    u8 amount_of_elements;
    u8 current_selection;
    u8 state;
} Menu;

extern const char* const menu_toggle_options[2];

#define MENU_TOGGLE(label_, initial_)                                          \
    {.label = (label_),                                                        \
     .options = menu_toggle_options,                                           \
     .selection_option = (initial_),                                           \
     .option_count = 2,                                                        \
     .type = MENU_TYPE_TOGGLE}

/* must be an array so its option count can be calculated here. */
#define MENU_MULTI(label_, choices_, initial_)                                 \
    {.label = (label_),                                                        \
     .options = (choices_),                                                    \
     .selection_option = (initial_),                                           \
     .option_count = (u8)ARRAY_SIZE(choices_),                                 \
     .type = MENU_TYPE_MULTI}

#define MENU_SET_VALUE(label_, min_, max_, symbol_to_set_, initial_)           \
    {.label = (label_),                                                        \
     .value = (initial_),                                                      \
     .option_count = sizeof(u16),                                              \
     .type = MENU_TYPE_VALUE_SET,                                              \
     .symbol_to_set = symbol_to_set_}

void UpdateMenu(Menu* menu);
void UpdateAllMenus(void);

extern Menu main_menu;

#endif /* MENU_H */
