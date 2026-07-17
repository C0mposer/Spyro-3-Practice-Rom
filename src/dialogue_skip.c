#include <types.h>
#include <symbols.h>
#include <buttons.h>


bool g_fastDialogueEnabled = false;
bool timer_skip = false;

JAL_HOOK(0x80055460) // Replacing Dialogue Update call in UpdateLogic
void QuickAdvanceDialogue()
{
    if (g_fastDialogueEnabled && !timer_skip)
    {
        isButtonPressed |= X_BUTTON;
        timer_skip = true;
    }
    else
    {
        timer_skip = false;
    }

    DialogueUpdate();
}