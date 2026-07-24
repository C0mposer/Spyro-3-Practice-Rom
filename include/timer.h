#ifndef TIMER_H
#define TIMER_H

typedef enum TimerState
{
    TIMER_RUNNING,
    TIMER_DISPLAYING,

}TimerState;

typedef enum TimerDisplayMode
{
    TIMER_ONLY_PAUSE,
    TIMER_ALWAYS

}TimerDisplayMode;

typedef enum ILExitState
{
    IL_EXIT_NONE,
    IL_EXIT_FADING,
    IL_EXIT_PROMPT

} ILExitState;

typedef enum ILExitType
{
    IL_EXIT_MENU,
    IL_EXIT_PORTAL,
    IL_EXIT_MENU_CUTSCENE

} ILExitType;

typedef struct Timer
{
    int timer;
    int secondsOnesPlace;
    int secondsTensPlace;
    int milisecondsTenthsPlace;
    int milisecondsHundrethsPlace;
    int minutes;

}Timer;

#endif /* TIMER_H */
