#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include "skip_title_screen.h"
#include "menu/menu.h"
#include <buttons.h>
#include <syscalls.h>

void ModMain(void) {
    SkipTitleScreenUpdate();
    MainUpdates();
    UpdateAllMenus();
}