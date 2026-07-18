#include <types.h>
#include <symbols.h>

extern bool titleScreenSkipStarted;
extern bool sunriseLoadStarted;

// Once the music begins playing, automatcially set the title screen state to 0x14, which
// begins the cutscene load. We then have to wait for the first cutscene load to start.
// ONLY THEN, can we call our own LoadLevel to sunrise. There is probably a way to set
// everything in memory up in a way to be able to simply call it, but it was easier to
// essentially put the game into a state where it's acceptable to call LoadLevel.
// We also change the title card of the first cutscene loading screen to avoid visual
// artifacting. See: skip_intro_cutscenes.s for that.
void SkipTitleScreenUpdate(void)
{
    const s32 timeToSkip = 0x800; // If we try to go to the first cutscene too fast, the game will show a black screen for like 10 seconds. This is just like when you start a run too fast, before waiting for music.
    const s32 offsetToLoadLevel = 0x7F;
    const s32 timeToLoadLevel = timeToSkip + offsetToLoadLevel; // Wait for the first cutscene to load in, before loading sunrise

    if (globalTimer > timeToSkip && !titleScreenSkipStarted)
    {
        titleScreenSkipStarted = true;
        titleScreenState = 0x13;
    }
    if (globalTimer > timeToLoadLevel && !sunriseLoadStarted)
    {
        sunriseLoadStarted = true;
        LoadLevel(3, 10);
    }
}
