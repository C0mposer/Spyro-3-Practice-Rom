#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include <gamestates.h>
#include "menu.h"

void UpdateMenuElements(Menu* menu)
{
    if (menu->amount_of_elements > 0)
    {
        s32 x1 = menu->x1;
        s32 x2 = menu->x2;

        s32 textbox_center = (x1 + x2) / 2;
        for (s32 i = 0; i < menu->amount_of_elements; i++)
        {
            s32 y = menu->y1 + 20 + 5 + (14 * i);
            s32 color[16];
            color[i] = i == menu->current_selection ? SELECTED_COLOR : UNSELECTED_COLOR;

            if (menu->elements[i].type == MENU_TYPE_TOGGLE)
            {
                if (!menu->elements[i].enabled)
                {
                    DrawTextCentered(menu->elements[i].text[0], textbox_center, y, color[i]);
                }
                else
                {
                    DrawTextCentered(menu->elements[i].text[1], textbox_center, y, color[i]);
                }
            }
        }

        // Update Selection
        if (isButtonPressed == RIGHT_BUTTON && !menu->elements[menu->current_selection].enabled)
        {
            menu->elements[menu->current_selection].enabled = true;
            PlaySound(11, 0, 0);
        }
        else if (isButtonPressed == LEFT_BUTTON && menu->elements[menu->current_selection].enabled)
        {
            PlaySound(11, 0, 0);
            menu->elements[menu->current_selection].enabled = false;

        }

        // Change selection
        if (isButtonPressed == UP_BUTTON && menu->current_selection > 0)
        {
            menu->current_selection--;
            PlaySound(10, 0, 0);
            if (menu->current_selection < 0)
            {
                menu->current_selection = 0;
            }
        }
        else if (isButtonPressed == DOWN_BUTTON && menu->current_selection < (menu->amount_of_elements - 1))
        {
            menu->current_selection++;
            PlaySound(10, 0, 0);
            if (menu->current_selection > 15)
            {
                menu->current_selection = 16;
            }
        }
    }
}

void AddMenuElement(Menu* menu, MenuElement element)
{
    menu->elements[menu->amount_of_elements] = element;

    menu->amount_of_elements++;
}