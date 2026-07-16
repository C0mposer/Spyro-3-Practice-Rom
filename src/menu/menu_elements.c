#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include "menu.h"

static void ChangeElementValue(MenuElement* element, bool move_right)
{
    u8 old_value = element->selection_option;

    if (element->type == MENU_TYPE_TOGGLE)
    {
        element->selection_option = move_right ? 1 : 0;
    }
    else if (move_right)
    {
        element->selection_option++;

        if (element->selection_option >= element->option_count)
        {
            element->selection_option = element->option_count - 1;
        }
    }
    else if (element->selection_option == 0)
    {
        element->selection_option = 0;
    }
    else
    {
        element->selection_option--;
    }

    if (element->selection_option != old_value)
    {
        PlaySound(11, 0, 0);
    }
}

void UpdateMenuElements(Menu* menu)
{
    s32 textbox_center;
    s32 column_offset;
    u8 i;

    if (menu->amount_of_elements == 0)
    {
        return;
    }

    textbox_center = (menu->x1 + menu->x2) / 2;
    column_offset = (menu->x2 - menu->x1) / 4;

    for (i = 0; i < menu->amount_of_elements; i++)
    {
        MenuElement* element = &menu->elements[i];
        s32 y = menu->y1 + 25 + (14 * i);
        s32 color = i == menu->current_selection ? SELECTED_COLOR : UNSELECTED_COLOR;

        DrawTextCentered(element->label, textbox_center - column_offset, y, color);
        DrawTextCentered(
            element->options[element->selection_option],
            textbox_center + column_offset,
            y,
            color);
    }

    if (isButtonPressed == RIGHT_BUTTON)
    {
        ChangeElementValue(&menu->elements[menu->current_selection], true);
    }
    else if (isButtonPressed == LEFT_BUTTON)
    {
        ChangeElementValue(&menu->elements[menu->current_selection], false);
    }

    if (isButtonPressed == UP_BUTTON && menu->current_selection > 0)
    {
        menu->current_selection--;
        PlaySound(10, 0, 0);
    }
    else if (isButtonPressed == DOWN_BUTTON &&
        menu->current_selection + 1 < menu->amount_of_elements)
    {
        menu->current_selection++;
        PlaySound(10, 0, 0);
    }

}
