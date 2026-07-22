#include <types.h>
#include <symbols.h>

#define MOBY_STATE_OFFSET      0x48
#define PLAYER_CONTROLLED_STATE 3
#define CHECKPOINT_PATCH_BYTE   1

extern u32 controlledMobyActive;  // 0x8007056C
extern u8* controlledMoby;        // 0x80070578

// Respawn as the correct side character
void PreserveSideCharacterForRespawn(void)
{
    u8* mobyState;

    if (controlledMobyActive != 1 || controlledMoby == NULL)
    {
        return;
    }

    mobyState = controlledMoby + MOBY_STATE_OFFSET;

    if (*mobyState != PLAYER_CONTROLLED_STATE)
    {
        return;
    }

    SaveCheckpointMemoryPatch(
        mobyState,
        CHECKPOINT_PATCH_BYTE,
        savedCheckpointData);

    SaveCheckpointMemoryPatch(
        mobyState,
        CHECKPOINT_PATCH_BYTE,
        currentCheckpointData);
}
