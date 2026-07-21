
#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include <buttons.h>
#include <hotkeys.h>
#include <gamestates.h>
#include <timer_options.h>
#include <timer.h>

TimerState IL_timerState = TIMER_RUNNING;
Timer IL_mainTimer;
int IL_mainTimerAtReset;
char IL_mainTimerAscii[10];
bool IL_isLoadComboPressed = false;
bool IL_preparingToStartTimer = false;

ILExitState IL_exitState = IL_EXIT_NONE;
ILExitType IL_exitType = IL_EXIT_MENU;
s32 IL_exitContext = -1;
u32 IL_exitResumeGamestate = GAMEPLAY;
bool IL_retrySelected = true;
u32 IL_previousGamestate = GAMEPLAY;
u32 IL_sourceLevelID = 0;
u32 IL_sourceSubLevelID = 0;
bool IL_portalFadeActive = false;

Timer currentBestTimeInLevel = {0};
char currentBestTimeInLevelAscii[10];
