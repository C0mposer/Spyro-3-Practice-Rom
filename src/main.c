#include <types.h>
#include <symbols.h>
#include <syscalls.h>
#include <buttons.h>
#include <gamestates.h>
#include <menu.h>

<<<<<<< HEAD
extern Menu main_menu;
extern MenuElement vibration_toggle;
extern MenuElement balls_toggle;
extern MenuElement cheat_toggle;
extern MenuElement test_toggle;

=======
>>>>>>> 38c1baa9fbd55833da954a0e362f4c0a73853998

void ModMain(void) {
    ONCE
    {
        AddMenuElement(&main_menu, vibration_toggle);
        AddMenuElement(&main_menu, balls_toggle);
        AddMenuElement(&main_menu, cheat_toggle);
        AddMenuElement(&main_menu, test_toggle);
    }

    UpdateMenu(&main_menu);
    UpdateMenuTriggers();
}
