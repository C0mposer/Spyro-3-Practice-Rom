#include <types.h>
#include <symbols.h>
#include <gamestates.h>
#include <timer.h>
#include <common.h>

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
        IL_exitState = IL_EXIT_FADING;
        IL_retrySelected = true;
        gamestate = FROZEN;
        IL_exitType = IL_EXIT_MENU;
    }
}

JAL_HOOK(0x80058368)
void BeginLevelLoadAfterSparxPatch(s32 ctx, s32 ctx2)
{
    if (g_ILTimerMode == 0) // Only allow exiting level normally when IL mode is off
    {
        LoadLevel(0, currentLevel - 8);
    }
    else
    {
        IL_exitContext = ctx;
        IL_exitState = IL_EXIT_FADING;
        IL_retrySelected = true;
        gamestate = FROZEN;
        IL_exitType = IL_EXIT_MENU_CUTSCENE;
        drawScreenBlack = 0xFF; // The fade effect doesn't show up until 1 frame later when doing this normally. So, doing it here makes it line up.
    }
}
