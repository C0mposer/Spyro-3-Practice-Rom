#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include "skip_title_screen.h"
#include "menu/menu.h"
#include <buttons.h>
#include <syscalls.h>
#include <difficulty.h>

void ModMain(int syncArg)
{
    ONCE //Init
    {
        optionsMusicVolumeSlider = 0;
    }
    //SkipTitleScreenUpdate();
    MainUpdates();
    //DifficultyUpdate();
    UpdateAllMenus();
    TimerUpdate();


    DrawSync(syncArg);        // Replaced Function Call, we must call it from our hook
}
