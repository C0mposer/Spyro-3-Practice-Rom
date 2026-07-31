#include <symbols.h>
#include <types.h>

#define GEM_HUNDREDS_PER_LEVEL ((const u8 *)0x80066F54)
#define EGGS_PER_LEVEL         ((const u8 *)0x80066F7C)

void GetLevelGemCompletion(char* text_buffer)
{
    u32 index = levelIndex;

    sprintf(text_buffer, "%d/%d Gems", gemsCollectedPerLevel[index], GEM_HUNDREDS_PER_LEVEL[index] * 100);
}

void GetLevelEggCompletion(char* text_buffer)
{
    u32 index = levelIndex;
    u32 eggFlags = eggsCollectedBitmask[index];
    u32 eggs = 0;

    do
    {
        eggs += eggFlags & 1;
        eggFlags >>= 1;
    }
    while (eggFlags);

    sprintf(text_buffer, "%d/%d Eggs", eggs, EGGS_PER_LEVEL[index]);
}
