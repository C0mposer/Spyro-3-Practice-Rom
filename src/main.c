#include <types.h>
#include <symbols.h>
#include <syscalls.h>
#include <buttons.h>
#include <gamestates.h>
#include <menu.h>

extern Menu main_menu;
extern MenuElement vibration_toggle;
extern MenuElement balls_toggle;
extern MenuElement cheat_toggle;
extern MenuElement test_toggle;


void ModMain(void) {


    ONCE
    {
        AddMenuElement(&main_menu, vibration_toggle);
        AddMenuElement(&main_menu, balls_toggle);
        AddMenuElement(&main_menu, cheat_toggle);
        AddMenuElement(&main_menu, test_toggle);
    }
    UpdateMenu(&main_menu);

    if (main_menu.elements[ROSETOY_TOGGLE].enabled)
    {
        printf_syscall("Rose Toy buzzing mmm\n");
    }
    else
    {
        printf_syscall("SEND ME UR LINK IN CSGO BRO\n");
    }

}
