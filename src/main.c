#include "menu/menu.h"
#include <buttons.h>
#include <difficulty.h>
#include <symbols.h>
#include <syscalls.h>
#include <types.h>
#include <gamestates.h>

extern bool hasLoadedCDCode;
extern bool hasCheckedExtraRam;

#ifdef INJECTION_ONLY
void ProxyTrainerUpdate(void);
#endif

void ModMain(int syncArg)
{
#ifndef INJECTION_ONLY
#if defined(VERSION10_PS1) || defined(VERSION10_PS2) || defined(VERSION10_PS2_IOP)
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
    //BoarProxyTrainer();
    MoonJumpUpdate();
    SetCorrectVehicleInBossUpdate();
    CategoryDefaultsUpdate();
    SorcLayoutForceUpdate();

    #ifdef VERSION10_PS1
    if (!hasCheckedExtraRam)
    {
        hasCheckedExtraRam = true;
        CheckIfHasExtraRam();
    }
    #endif

// #ifdef INJECTION_ONLY
//     ProxyTrainerUpdate();
// #endif
//     //SorceressMapUpdate();

    DrawSync(syncArg); // Replaced Function Call, we must call it from our hook
}
