#include <buttons.h>
#include <symbols.h>
#include <types.h>

extern bool g_fastDialogueEnabled;
extern g_ILTimerMode;

extern bool timer_skip;

JAL_HOOK(0x80055460) // Replacing Dialogue Update call in UpdateLogic
void QuickAdvanceDialogue()
{
    if (g_fastDialogueEnabled) // Is menu option enabled
    {
        if (g_ILTimerMode == 0) // Only allow dialogue mash with IL mode off
        {
            if (!timer_skip) // Skip dialouge only every other frame, to feel more human
            {
                isButtonPressed |= X_BUTTON;
                timer_skip = true;
            }
            else // Dialogue will be skipped next frame
            {
                timer_skip = false;
            }
        }
    }

    DialogueUpdate();
}
