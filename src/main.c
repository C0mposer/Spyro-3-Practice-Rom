#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include "skip_title_screen.h"
#include "menu/menu.h"
#include <buttons.h>
#include <syscalls.h>
#include <difficulty.h>

extern bool hasLoadedCDCode;
#define KERN1_FILE_SECTOR 253783

#ifdef VERSION10_PS1
#define KERN1_MEMORY_ADDRESS 0x8000C000
#elif VERSION10_PS2
#define KERN1_MEMORY_ADDRESS 0x80008EB0
#endif

void ModMain(int syncArg)
{
    // if (globalTimer > 350 && !hasLoadedCDCode)
    // {
    //     printf_syscall("Read CD Data\n");
    //     ReadCdSync(253783, KERN1_MEMORY_ADDRESS, 0x1000, 0);
    //     hasLoadedCDCode = true;
    // }
    // else if (hasLoadedCDCode) // Run code after KERN1.BIN has been loaded from the CD
    // {
    SkipTitleScreenUpdate();
    UpdateAllMenus();
    MainUpdates();
    TimerUpdate();
    RhynocProxyTrainer();
// }

//DifficultyUpdate();

    DrawSync(syncArg);        // Replaced Function Call, we must call it from our hook
}
