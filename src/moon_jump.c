#include <types.h>
#include <symbols.h>
#include <buttons.h>

extern g_ILTimerMode;
void MoonJumpUpdate()
{
    if (g_ILTimerMode == 0) // Only when not in IL mode
    {
        if (isButtonHeld == (L1_BUTTON + R1_BUTTON + L2_BUTTON + R2_BUTTON + X_BUTTON))
        {
            spyroZ += 500;
        }
    }
}
