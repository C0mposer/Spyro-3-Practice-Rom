#include <types.h>
#include <symbols.h>
#include <timer.h>
#include <gamestates.h>

extern bool g_shouldDisablePortal;
extern u32 g_ILTimerMode;
extern bool fastLoadActive;
extern bool fastLoadInScenario;
extern bool speedUpResetPending;
extern TimerState timerState;

extern bool shouldSaveTimerPortal;
extern ILExitState IL_exitState;
extern ILExitType IL_exitType;
extern u32 IL_exitResumeGamestate;
extern bool IL_portalFadeActive;

JAL_HOOK(0x800554FC)
void ILLoadingGlideUpdatePatch(void)
{
    if (!IL_portalFadeActive)
    {
        UpdateDuringLoadingGlide();
    }
}

// UpdateDuringLoadingGlide normally starts to read from the disc on its first frame, while Spyro is still gliding into the portal. Stop that from happening, as it will break restarting the level
JAL_HOOK(0x800544B0)
void ILPortalLoadPatch(void)
{
    if (IL_exitState != IL_EXIT_FADING || IL_exitType != IL_EXIT_PORTAL)
    {
        LoadLevelRelated();
    }
}

static bool UpdateILPortalTransition(void)
{
    u32 fadeFrame;
    s32 distance;

    // LoadingGlide normally replaces the level overlay after a 32 frame glide, then the common overlay performs the visible portal fade. Freeze right before, and mimic just the visual part of exiting a portl.
    if (!IL_portalFadeActive)
    {
        if (framesInScenario < 0x20)
        {
            return false;
        }

        IL_portalFadeActive = true;
        framesInScenario = 0;
        return false;
    }

    framesInScenario++;

    // The game normally spends two ticks advancing menuState 1 -> 2 before fading. (idk why it's called menu state when it's more like a loading state?)
    if (framesInScenario < 3)
    {
        return false;
    }

    fadeFrame = framesInScenario - 2;
    if (fadeFrame < 0x10)
    {
        distance = MoveSpyroTowardPoint(&portalGlideTarget);
        if (distance < 0x400)
        {
            portalGlideTarget.x += portalGlideDirection.x;
            portalGlideTarget.y += portalGlideDirection.y;
        }
        UpdateSpyroAnimation();
        drawScreenBlack = fadeFrame << 4;
        return false;
    }

    return true;
}

// Do just the visual exit fade
bool UpdateILExitFade(void)
{
    if (IL_exitType == IL_EXIT_PORTAL)
    {
        return UpdateILPortalTransition();
    }
    else if (IL_exitType == IL_EXIT_MENU_CUTSCENE) // Immediately be fully black on cutscene exit (like hunters tussle)
    {
        drawScreenBlack = 0xFF;
        return true;
    }
    else if (IL_exitType == IL_EXIT_MENU)
    {

        if (drawScreenBlack < 0xFF)
        {
            drawScreenBlack += 0x10;
            if (drawScreenBlack > 0xFF)
            {
                drawScreenBlack = 0xFF;
            }
        }
        return drawScreenBlack >= 0xF8;
    }
}

void EndILPortalTransition(bool resumeLoad)
{
    IL_portalFadeActive = false;
    if (!resumeLoad)
    {
        return;
    }

    if (IL_exitResumeGamestate == LOADING_LEVEL)
    {
        framesInScenario = 0x20;
        currentMenu = 1;
    }
    gamestate = IL_exitResumeGamestate;
}

JAL_HOOK(0x80040230) // Hooking the call to BeginLevelLoad in HandleTriggerCollision
void EnterPortalPatch(s32 context)
{
    // Actually load the level
    if (!g_shouldDisablePortal || g_ILTimerMode > 0)
    {
        BeginLevelLoad(context);
    }
    else
    {
        //DrawText("Touched Portal!", 20, 20, 1, 0);
        ClearCollectables();
        PrepareSavedSpyroRespawn();
        speedUpResetPending = true;
        if (FastLoadEnabled())
        {
            fastLoadActive = true;
            fastLoadInScenario = false;
        }
        RespawnSpyro();

        shouldSaveTimerPortal = true;
        return;
    }
}
