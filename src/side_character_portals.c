#include <types.h>
#include <symbols.h>

void UnlockAllSideCharacterPortals()
{
    eggsCollectedBitmask[0x6] = 0b1; // Set sheila egg as collected
    eggsCollectedBitmask[0xF] = 0b1; // Set sgt byrd egg as collected
    eggsCollectedBitmask[0x18] = 0b1; // Set bently egg as collected
    eggsCollectedBitmask[0x21] = 0b100; // Set agent 9 egg as collected
}
