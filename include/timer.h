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

#endif /* TIMER_H */
