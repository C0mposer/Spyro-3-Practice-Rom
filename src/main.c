#include "menu/menu.h"
#include "skip_title_screen.h"
#include <buttons.h>
#include <difficulty.h>
#include <symbols.h>
#include <syscalls.h>
#include <types.h>

extern bool hasLoadedCDCode;
#define KERN1_FILE_SECTOR 253783

#ifdef VERSION10_PS1
#define KERN1_MEMORY_ADDRESS 0x8000C000
#elif VERSION10_PS2
#define KERN1_MEMORY_ADDRESS 0x80008EB0
#endif

void ModMain(int syncArg)
{
    #ifndef INJECTION_ONLY
    if (globalTimer > 350 && !hasLoadedCDCode)
    {
        printf_syscall("Read CD Data\n");
        ReadCdSync(253783, KERN1_MEMORY_ADDRESS, 0x1800, 0);
        hasLoadedCDCode = true;
    }
    else if (hasLoadedCDCode) // Run code after KERN1.BIN has been loaded from
                              // the CD
    {
        #endif

        #ifndef INJECTION_ONLY
        SkipTitleScreenUpdate();
        #endif
        UpdateAllMenus();
        MainUpdates();
        TimerUpdate();
        ButtonsHeldTimerUpdate();
        RhynocProxyTrainer();

        #ifndef INJECTION_ONLY
    }
    #endif

    DrawSync(syncArg); // Replaced Function Call, we must call it from our hook
}
