#include <types.h>
#include <symbols.h>
#include <gamestates.h>
#include <timer.h>

extern u32 g_ILTimerMode;
extern ILExitState IL_exitState;
extern ILExitType IL_exitType;
extern s32 IL_exitContext;
extern bool IL_retrySelected;

JAL_HOOK(0x80058378)
void BeginLevelLoadPatch(s32 ctx)
{
    if (g_ILTimerMode == 0) // Only allow exiting level normally when IL mode is off
    {
        BeginLevelLoad(ctx);
    }
    else
    {
        IL_exitContext = ctx;
        IL_exitType = IL_EXIT_MENU;
        IL_exitState = IL_EXIT_FADING;
        IL_retrySelected = true;
        gamestate = FROZEN;
    }
}
