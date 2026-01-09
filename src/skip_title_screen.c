#include <types.h>
#include <symbols.h>

// Once the music begins playing, automatcially set the title screen state to 0x14, which
// begins the cutscene load. We then have to wait for the first cutscene load to start.
// ONLY THEN, can we call our own LoadLevel to sunrise. There is probably a way to set
// everything in memory up in a way to be able to simply call it, but it was easier to
// essentially put the game into a state where it's acceptable to call LoadLevel.
// We also change the title card of the first cutscene loading screen to avoid visual
// artifacting. See: skip_intro_cutscenes.s for that. 
void SkipTitleScreenUpdate(void)
{
    if (globalTimer > 0x580)
    {
        ONCE
        {
            titleScreenState = 0x13;
        }
    }
    if (globalTimer > 0x5FA)
    {
        ONCE
        {
            LoadLevel(3, 10);
        }
    }
}