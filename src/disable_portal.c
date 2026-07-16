#include <types.h>
#include <symbols.h>
#include <timer.h>

extern bool g_shouldDisablePortal;
extern bool fastLoadActive;
extern bool fastLoadInScenario;
extern bool speedUpResetPending;
extern TimerState timerState;

bool shouldSaveTimerPortal = false;

JAL_HOOK(0x80040230) // Hooking the call to BeginLevelLoad in HandleTriggerCollision
void EnterPortalPatch(void* context)
{
    if (!g_shouldDisablePortal)
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