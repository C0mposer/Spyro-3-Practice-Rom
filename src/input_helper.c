#include <types.h>
#include <symbols.h>
#include <buttons.h>

// This function keeps track of a single timer for how long the current input have been held for, and resets anytime it changes.
// I realized I consistently create specific timers for checking held buttons for different purposes, but a generic system could be useful here.

extern u32 framesCurrentButtonsHeld;
extern u32 buttonsHeldLastFrame;

void ButtonsHeldTimerUpdate()
{
    // If you're holding nothing, reset the timer
    if (isButtonHeld == 0)
    {
        framesCurrentButtonsHeld = 0;
    }
    // If the current buttons held this frame are the same as last frame, count up
    else if (buttonsHeldLastFrame == isButtonHeld)
    {
        framesCurrentButtonsHeld++;
    }
    // If they're not the same, reset the timer
    else if (buttonsHeldLastFrame != isButtonHeld)
    {
        framesCurrentButtonsHeld = 0;
    }

    buttonsHeldLastFrame = isButtonHeld; // <- isButtonHeld is just the current buttons.
}

u32 HeldButtonsFrameTimer()
{
    return framesCurrentButtonsHeld;
}

// Example: HasHeldButton(TRIANGLE, 5) // Has held triangle for 5 frames? Returns true if so
bool HasHeldButton(u32 button, u32 timeInFrames)
{
    if (buttonsHeldLastFrame == button)
    {
        if (framesCurrentButtonsHeld >= timeInFrames)
        {
            return true;
        }
    }

    return false;
}
