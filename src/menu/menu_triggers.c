#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include "menu.h"
#include "menu_triggers.h"
#include "../controllerviewer.h"

// Update the Off/On logic
void UpdateMenuTriggers()
{
    WaffleTrigger();
    VibrationTrigger();
    DerekTrigger();
}


// Logic for the different Off/On switches in the menu
void VibrationTrigger()
{
    if (main_menu.elements[VIBRATION_TOGGLE].enabled)
    {
        printf_syscall("Vibration Mode On\n\n");
    }
    else
    {
        printf_syscall("Vibration Mode Off\n\n");
    }
}

void DerekTrigger()
{
    if (main_menu.elements[WAFFLE_TOGGLE].enabled)
    {
        printf_syscall("Waffle's server is on. Shocker!\n\n");
    }
    else
    {
        printf_syscall("Waffle's server is off. No shock there.\n\n");
    }
}

void WaffleTrigger()
{
    if (main_menu.elements[DEREK_TOGGLE].enabled)
    {
        //ControllerViewer();
    }
    else
    {
        printf_syscall("My balls don't itch somehow\n\n");
    }
}