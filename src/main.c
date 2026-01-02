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
    UpdateMenuTriggers();
}
