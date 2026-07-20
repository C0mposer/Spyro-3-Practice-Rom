#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include <buttons.h>
#include <hotkeys.h>
#include <gamestates.h>
#include <timer_options.h>
#include <timer.h>

TimerState timerState = TIMER_RUNNING;
Timer mainTimer;
int mainTimerAtReset;
char mainTimerAscii[10];

bool isLoadComboPressed = false;

bool preparingToStartTimer = false;
