#include <types.h>
#include <symbols.h>

bool doesHaveExtraRam = false;
void CheckIfHasExtraRam()
{
    raw_memory_ptr* startOfRam = 0x80000000;
    raw_memory_ptr* startOfExtraRam = 0x80200000;
    if (*startOfRam == *startOfExtraRam) // If 80000000 and 80200000 are the same, the psx is wrapping around at the 2mb boundary, so no extra ram
    {
        doesHaveExtraRam = false;
    }
    else
    {
        doesHaveExtraRam = true;
    }
}
