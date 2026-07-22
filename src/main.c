#include "menu/menu.h"
#include <buttons.h>
#include <difficulty.h>
#include <symbols.h>
#include <syscalls.h>
#include <types.h>
#include <gamestates.h>

extern bool hasLoadedCDCode;

void ModMain(int syncArg)
{
#ifndef INJECTION_ONLY
#if defined(VERSION10_PS1) || defined(VERSION10_PS2) || defined(VERSION10_VITA)
    if (!hasLoadedCDCode)
    {
        drawScreenBlack = 0xFF;
        BootUpdate();
        if (hasLoadedCDCode) { drawScreenBlack = 0; }
        DrawSync(syncArg);
        return;
    }
#endif
#endif

    UpdateAllMenus();
    MainUpdates();
    ILTimerUpdate();
    TimerUpdate();
    ButtonsHeldTimerUpdate();
    RhynocProxyTrainer();
    MoonJumpUpdate();
    SetCorrectVehicleInBossUpdate();
    CategoryDefaultsUpdate();

    DrawSync(syncArg); // Replaced Function Call, we must call it from our hook
}
