#include "menu.h"
#include <buttons.h>
#include <symbols.h>
#include <types.h>
#include <input_helper.h>

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
    else if (element->selection_option == 0) { element->selection_option = 0; }
    else
    {
        element->selection_option--;
    }

    if (element->selection_option != old_value) { PlaySound(11, 0, 0); }
}

static void SetValueFromElement(MenuElement* element)
{
    u32 value_to_set = element->value;

    PlaySound(11, 0, 0);
    *(element->symbol_to_set) = value_to_set;
    //printf_syscall("Setting pointer: %X with value %d\n",
    //(element->symbol_to_set), value_to_set);
}

void UpdateMenuElements(Menu* menu)
{
    s32 textbox_center;
    s32 column_offset;
    u8 page_element_count;
    u8 i;

    page_element_count = GetMenuPageElementCount(menu);

    if (page_element_count == 0) { return; }

    textbox_center = (menu->x1 + menu->x2) / 2;
    column_offset = (menu->x2 - menu->x1) / 4;

    for (i = 0; i < page_element_count; i++)
    {
        MenuElement* element = GetMenuPageElement(menu, i);
        s32 y = menu->y1 + 25 + (14 * i);
        s32 color =
            i == menu->current_selection ? SELECTED_COLOR : UNSELECTED_COLOR;

        DrawTextCentered(element->label, textbox_center - column_offset, y,
                         color);

        // Draw actual value int as string if Value Set type
        if (element->type == MENU_TYPE_VALUE_SET)
        {
            char buffer[12];
            sprintf(buffer, "%d", element->value);
            DrawTextCentered(buffer, textbox_center + column_offset, y, color);
        }

        // Draw String from selection array otherwise
        else
        {
            DrawTextCentered(element->options[element->selection_option],
                             textbox_center + column_offset, y, color);
        }
    }

    MenuElement* current_element =
        GetMenuPageElement(menu, menu->current_selection);

    // Change Option/Set Value for the Set Value menu type
    if (current_element->type == MENU_TYPE_VALUE_SET)
    {
        // There is certainly a better way to do this. But this will suffice for now
        if (isButtonPressed == LEFT_BUTTON)
        {
            current_element->value -= 1;
        }
        else if (isButtonPressed == RIGHT_BUTTON)
        {
            current_element->value += 1;
        }
        else if (isButtonPressed == L2_BUTTON)
        {
            current_element->value -= 100;
        }
        else if (isButtonPressed == R2_BUTTON)
        {
            current_element->value += 100;
        }
        else if (HasHeldButton(LEFT_BUTTON, 90))
        {
            current_element->value -= 200;
        }
        else if (HasHeldButton(RIGHT_BUTTON, 90))
        {
            current_element->value += 200;
        }
        else if (HasHeldButton(LEFT_BUTTON, 10))
        {
            current_element->value -= 2;
        }
        else if (HasHeldButton(RIGHT_BUTTON, 10))
        {
            current_element->value += 2;
        }
        else if (HasHeldButton(L2_BUTTON, 10))
        {
            current_element->value -= 200;
        }
        else if (HasHeldButton(R2_BUTTON, 10))
        {
            current_element->value += 200;
        }

        // Set value when exiting the menu
        else if (isButtonPressed & (X_BUTTON | TRIANGLE_BUTTON | CIRCLE_BUTTON))
        {
            SetValueFromElement(current_element);
        }
    }

    // Change option for toggle/multi
    else
    {
        if (isButtonPressed == RIGHT_BUTTON)
        {
            ChangeElementValue(current_element, true);
        }
        else if (isButtonPressed == LEFT_BUTTON)
        {
            ChangeElementValue(current_element, false);
        }
    }

    // Scroll Menu
    if (isButtonPressed == UP_BUTTON)
    {
        // Don't underflow
        if (menu->current_selection > 0) { menu->current_selection--; }
        else // Wrap around
        {
            menu->current_selection = page_element_count - 1;
        }
        PlaySound(10, 0, 0);
    }
    else if (isButtonPressed == DOWN_BUTTON)
    {
        if (menu->current_selection + 1 < page_element_count)
        {
            menu->current_selection++;
        }
        else
        {
            menu->current_selection = 0;
        }
        PlaySound(10, 0, 0);
    }
}
