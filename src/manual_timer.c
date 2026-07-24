#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include <buttons.h>
#include <hotkeys.h>
#include <gamestates.h>
#include <timer_options.h>
#include <timer.h>
#include <text_colors.h>

extern int g_manualTimerMode;
extern int menu_frames_closed;
extern bool shouldSaveTimerPortal;

extern TimerState timerState;
extern Timer mainTimer;
extern int mainTimerAtReset;
extern char mainTimerAscii[10];
extern bool isLoadComboPressed;
extern bool preparingToStartTimer;

extern u32 g_ILTimerMode;

//Math to approx adjust for 59.82hz
void FramesToTimer(Timer* ptr_timer)
{
    ptr_timer->minutes = (ptr_timer->timer * 10) / 35892;
    ptr_timer->secondsTensPlace = ((ptr_timer->timer * 10) % 35892) / 5982;
    ptr_timer->secondsOnesPlace = ((ptr_timer->timer * 100) % 59820) / 5982;
    ptr_timer->milisecondsTenthsPlace = ((ptr_timer->timer * 1000) % 59820) / 5982;
    ptr_timer->milisecondsHundrethsPlace = ((ptr_timer->timer * 10000) % 59820) / 5982;
}

void LoadAscii(Timer* ptr_timer, char* ascii) {
    if (ptr_timer->minutes == 0) {
        sprintf(ascii, "%d%d.%d%d", ptr_timer->secondsTensPlace, ptr_timer->secondsOnesPlace, ptr_timer->milisecondsTenthsPlace, ptr_timer->milisecondsHundrethsPlace);
    }
    else if (ptr_timer->minutes >= 1) {
        sprintf(ascii, "%d.%d%d.%d%d", ptr_timer->minutes, ptr_timer->secondsTensPlace, ptr_timer->secondsOnesPlace, ptr_timer->milisecondsTenthsPlace, ptr_timer->milisecondsHundrethsPlace);
    }
}

//! Every Frame Update
void TimerUpdate()
{
    if (g_manualTimerMode > 0) // Is > 0, aka not off
    {
        //Main Timer Checks/Loop

        //Button Checks
        if ((isButtonHeld == LOAD_SPYRO_HOTKEY || isButtonHeld == RELOAD_LEVEL_HOTKEY) && isLoadComboPressed == false)
        {
            //mainTimerAtReset = globalTimer;
            //timerState = TIMER_RUNNING;
            preparingToStartTimer = true;
            isLoadComboPressed = true;
        }
        if (isButtonHeld != LOAD_SPYRO_HOTKEY && isLoadComboPressed == true)
        {
            isLoadComboPressed = false;
        }
        if (preparingToStartTimer)
        {
            if (gamestate == GAMEPLAY)
            {
                mainTimerAtReset = globalTimer;
                timerState = TIMER_RUNNING;
                preparingToStartTimer = false;
            }
        }
        // If you pause exited a level, put timer back in running state to not show it after the exit/loop
        if (gamestate == LOADING_LEVEL)
        {
            mainTimerAtReset = globalTimer;
            timerState = TIMER_RUNNING;
        }

        // Calculate the current time as ascii when paused, or every frame with TIMER_SHOW_ALWAYS
        if (timerState == TIMER_RUNNING)
        {
            if (isButtonPressed == SELECT_BUTTON || shouldSaveTimerPortal == true || g_manualTimerMode == TIMER_SHOW_ALWAYS)
            {
                mainTimer.timer = globalTimer - mainTimerAtReset;
                FramesToTimer(&mainTimer);
                LoadAscii(&mainTimer, mainTimerAscii);
                //printf_syscall("Should Be Calcing");
            }
        }


        if (isButtonPressed == SELECT_BUTTON || shouldSaveTimerPortal == true) //|| !hasUpdatedPortalTimer) // Add portal for this eventually
        {
            timerState = TIMER_DISPLAYING;
            shouldSaveTimerPortal = false;
        }

        // Display the timer
        if ((g_manualTimerMode == TIMER_SHOW_ALWAYS || timerState == TIMER_DISPLAYING) && (gamestate == GAMEPLAY || gamestate == PAUSED) && menu_frames_closed > 2) // Have to wait 2 frames after the menu closes to start displaying
        {
            //Textbox coords
            int x1 = 415;
            int x2 = 504;
            int y1 = 204;
            int y2 = 224;
            int text_y = 210;

            if (g_ILTimerMode == 0)
            {
                y1 = 204;
                y2 = 224;
                text_y = 210;
            }
            else // Raise the manual timer if in IL mode
            {
                y1 = 204 - 22;
                y2 = 224 - 22;
                text_y = 210 - 22;
            }

            //Text X adjust
            int text_x_pos_adjust;
            if (mainTimer.minutes == 0)
            {
                text_x_pos_adjust = 476;
                x1 = 442;
                x2 = 508;
            }
            else
            {
                x1 = 413;
                x2 = 504;
                text_x_pos_adjust = 460;
            }
            DrawTextbox(x1, x2, y1, y2);
            DrawTextCentered(mainTimerAscii, text_x_pos_adjust, text_y, COLOR_BRONZE);
            //printf_syscall("Should Be Drawing");
        }
    }
    //printf_syscall("timerState = %d, manualTimerMode = %d, isLoadComboPressed = %d\n", timerState, g_manualTimerMode, isLoadComboPressed);
}
