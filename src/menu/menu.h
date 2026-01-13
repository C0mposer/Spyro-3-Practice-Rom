#ifndef MENU_H
#define MENU_H

#define UNSELECTED_COLOR 0x4
#define SELECTED_COLOR 0x6

// Enums
enum MenuTypes
{
    MENU_TYPE_TOGGLE,
    MENU_TYPE_MULTI
};

typedef enum
{
    MENU_STATE_CLOSED = 0,
    MENU_STATE_OPENING,
    MENU_STATE_OPEN,
    MENU_STATE_CLOSING
} MenuState;

enum
{
    VIBRATION_TOGGLE,
    WAFFLE_TOGGLE,
    DEREK_TOGGLE
};

// Scructs
struct MenuElement
{
    const char* text[16];
    union
    {
        bool enabled;
        s32 selection_option;
    };
    s32 type;
};
typedef struct MenuElement MenuElement;

struct Menu
{
    const char* title;
    s32 x1, x2, y1, y2;

    MenuState state;

    MenuElement elements[16];
    s32 amount_of_elements;
    s32 current_selection;
};
typedef struct Menu Menu;

// Prototypes
void UpdateMenu(Menu* menu);
void UpdateAllMenus();

// Include these global menu's and menu elements with anything that includes menu.h
extern Menu main_menu;
extern MenuElement vibration_toggle;
extern MenuElement balls_toggle;
extern MenuElement cheat_toggle;

#endif /* MENU_H */
