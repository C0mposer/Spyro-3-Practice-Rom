#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include <menu.h>
#include <syscalls.h>
#include <gamestates.h>

// Menu Data
Menu main_menu = { .title = "* Waffle Menu v1.0 *", .x1 = 114, .x2 = 400, .y1 = 60, .y2 = 180, .amount_of_elements = 0, .current_selection = 0 };

MenuElement vibration_toggle = { .type = MENU_TYPE_TOGGLE, .enabled = false, .text[0] = "Vibration Off", .text[1] = "Vibration On" };
MenuElement balls_toggle = { .type = MENU_TYPE_TOGGLE, .enabled = false, .text[0] = "Rose Toy Off", .text[1] = "Rose Toy On" };
MenuElement cheat_toggle = { .type = MENU_TYPE_TOGGLE, .enabled = false, .text[0] = "Dereklander Chatmode Off", .text[1] = "Dereklander Chatmode On" };
MenuElement test_toggle = { .type = MENU_TYPE_TOGGLE, .enabled = true, .text[0] = "Squirt machine off", .text[1] = "Squirt machine on!!**" };






// Menu Logic
void UpdateMenu(Menu* menu)
{
    if (buttonJoker == TRIANGLE_BUTTON && !menu->is_open)
    {
        menu->is_open = true;
        gamestate = fireworksMatrixCutscene;
        PlaySound(14, 0, 0);

        if (musicVolume != 0)
        {
            musicVolume /= 4;
        }
        
    }
    else if (buttonJoker == TRIANGLE_BUTTON && menu->is_open)
    {
        menu->is_open = false;
        gamestate = gameplay;
        PlaySound(7, 0, 0);
        
        if (musicVolume != 0)
        {
            musicVolume *= 4;
        }
    }


    if (menu->is_open == true)
    {
        DrawTextbox(menu->x1, menu->x2, menu->y1, menu->y2);

        int line_x1 = menu->x1 + 10;
        int line_y1 = menu->y1 + 20;
        int line_x2 = menu->x2 - 10;
        int line_y2 = menu->y1 + 20;
        DrawShadedLine(line_x1, line_y1, line_x2, line_y2);

        int line_center = (line_x1 + line_x2) / 2;
        DrawTextCentered(menu->title, line_center, line_y1 - 13, 0x4);

        UpdateMenuElements(menu);

    }
}

void UpdateMenuElements(Menu* menu)
{
    if (menu->amount_of_elements > 0)
    {
        int x1 = menu->x1;
        int x2 = menu->x2;

        int textbox_center = (x1 + x2) / 2;
        for (int i = 0; i < menu->amount_of_elements; i++)
        {
            int y = menu->y1 + 20 + 5 + (14 * i);
            int color[16];
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
