#include "menu.h"
#include <buttons.h>
#include <symbols.h>
#include <text_colors.h>

const char* const main_menu_page_titles[MAIN_MENU_PAGE_COUNT] = {
    "* Practice Menu *",
    "* Savestate Menu *",
    "* Quality of Life *",
    "* Specific Menu *"
};

void DrawMenuPageControls(const Menu* menu)
{
    s32 y;

    if (menu->amount_of_pages < 2) { return; }

    y = menu->y2 - 15;
    DrawTextCentered("Prev L1", menu->x1 + 44, y, COLOR_CREAM);
    DrawTextCentered("R1 Next", menu->x2 - 44, y, COLOR_CREAM);
}

u8 GetMenuPageElementCount(const Menu* menu)
{
    u8 count = 0;
    u8 i;

    for (i = 0; i < menu->amount_of_elements; i++)
    {
        if (menu->elements[i].page == menu->current_page) { count++; }
    }

    return count;
}

MenuElement* GetMenuPageElement(Menu* menu, u8 page_index)
{
    u8 visible_index = 0;
    u8 i;

    for (i = 0; i < menu->amount_of_elements; i++)
    {
        MenuElement* element = &menu->elements[i];

        if (element->page != menu->current_page) { continue; }

        if (visible_index == page_index) { return element; }
        visible_index++;
    }

    return 0;
}

void UpdateMenuPage(Menu* menu)
{
    u8 new_page = menu->current_page;

    if (menu->amount_of_pages < 2) { return; }

    if (isButtonPressed == L1_BUTTON)
    {
        if (new_page == 0) { new_page = menu->amount_of_pages - 1; }
        else { new_page--; }
    }
    else if (isButtonPressed == R1_BUTTON)
    {
        new_page++;
        if (new_page >= menu->amount_of_pages) { new_page = 0; }
    }
    else
    {
        return;
    }

    menu->current_page = new_page;
    menu->current_selection = 0;
    PlaySound(14, 0, 0);
}
