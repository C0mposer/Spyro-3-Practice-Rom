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

enum
{
    VIBRATION_TOGGLE,
    BALLS_TOGGLE,
    CHEAT_TOGGLE
};

// Scructs
struct MenuElement
{
    const char* text[16];
    bool enabled;
    int selection_option;
    int type;
};
typedef struct MenuElement MenuElement;

struct Menu
{
    const char* title;
    bool is_open;
    MenuElement elements[16];
    int amount_of_elements;
    int current_selection;
    int x1;
    int x2;
    int y1;
    int y2;
};
typedef struct Menu Menu;

// Prototypes
void UpdateMenu(Menu* menu);
void AddMenuElement(Menu* menu, MenuElement element);

// Include these global menu's and menu elements with anything that includes menu.h
extern Menu main_menu;
extern MenuElement vibration_toggle;
extern MenuElement balls_toggle;
extern MenuElement cheat_toggle;

#endif /* MENU_H */
