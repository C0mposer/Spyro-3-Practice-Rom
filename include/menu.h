#ifndef MENU_H
#define MENU_H

enum
{
    MENU_TYPE_TOGGLE,
    MENU_TYPE_MULTI
};

enum
{
    UNSELECTED_COLOR = 0x4,
    SELECTED_COLOR = 0x6
};

enum
{
    VIBRATION_TOGGLE,
    ROSETOY_TOGGLE,
};

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

void UpdateMenu(Menu* menu);
void AddMenuElement(Menu* menu, MenuElement element);

#endif /* MENU_H */
