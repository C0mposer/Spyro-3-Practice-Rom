#include <symbols.h>
#include <types.h>

enum
{
    KERN1_FILE_SECTOR = 253783,
    KERN1_READ_SIZE = 0x1800,
    SUNRISE_SPRING_LEVEL_ID = 10,
    TITLE_STATE_PREPARE_SELECTED_GAME = 0x13,
    TITLE_STATE_START_SELECTED_GAME = 0x14
};

#ifdef VERSION10_PS1
#define KERN1_MEMORY_ADDRESS ((u32*)0x8000C000)
#elif defined(VERSION10_PS2)
#define KERN1_MEMORY_ADDRESS ((u32*)0x80008EB0)
#endif

extern bool hasLoadedCDCode;

// Used to load KERN.bin from the disc at a safe time, and to skip the title screen
void BootUpdate(void)
{
    if (titleScreenState == 1)
    {
        currentLevel = SUNRISE_SPRING_LEVEL_ID;
        titleScreenState = TITLE_STATE_PREPARE_SELECTED_GAME;
        titleScreenPreviousState = TITLE_STATE_PREPARE_SELECTED_GAME;
        titleScreenSubstate = 0;
        return;
    }

    if (titleScreenState != TITLE_STATE_START_SELECTED_GAME)
    {
        return;
    }

    ReadCdSync(KERN1_FILE_SECTOR, KERN1_MEMORY_ADDRESS, KERN1_READ_SIZE, 0);
    FlushCache();
    hasLoadedCDCode = true;
}
