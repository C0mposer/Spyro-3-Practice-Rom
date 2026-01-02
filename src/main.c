#include <types.h>
#include <symbols.h>
#include <syscalls.h>
#include <buttons.h>
#include <gamestates.h>
#include <menu.h>


void ModMain(void) {
    ONCE
    {
        AddMenuElement(&main_menu, vibration_toggle);
        AddMenuElement(&main_menu, balls_toggle);
        AddMenuElement(&main_menu, cheat_toggle);
    }

    UpdateMenu(&main_menu);

    // Example triggers from the menu
    if (main_menu.elements[BALLS_TOGGLE].enabled)
    {
        printf_syscall("Rose Toy buzzing mmm\n");
    }
    else
    {
        printf_syscall("SEND ME UR LINK IN CSGO BRO\n");
    }

}
