#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include <buttons.h>
#include <hotkeys.h>
#include <gamestates.h>
#include <timer_options.h>
#include <timer.h>
#include <common.h>
#include <text_colors.h>
#include <level_ids.h>

extern int g_ILTimerMode;
extern int menu_frames_closed;
extern bool shouldSaveTimerPortal;

extern Timer currentBestTimeInLevel;
extern char currentBestTimeInLevelAscii[10];

extern TimerState IL_timerState;
extern Timer IL_mainTimer;
extern int IL_mainTimerAtReset;
extern char IL_mainTimerAscii[10];
extern bool IL_isLoadComboPressed;
extern bool IL_preparingToStartTimer;

extern ILExitState IL_exitState;
extern ILExitType IL_exitType;
extern s32 IL_exitContext;
extern u32 IL_exitResumeGamestate;
extern bool IL_retrySelected;
extern u32 IL_previousGamestate;
extern u32 IL_sourceLevelID;
extern u32 IL_sourceSubLevelID;

void RestartLevelFromBeginning(void);
void FramesToTimer(Timer* ptr_timer);
void LoadAscii(Timer* ptr_timer, char* ascii);
void ILTimerFinishedUpdate(void);
bool UpdateILExitFade(void);
void EndILPortalTransition(bool resumeLoad);

static void DetectILPortalTransition(void)
{
    bool enteredMainLoad = gamestate == LOADING_LEVEL && IL_previousGamestate != LOADING_LEVEL;

    // IL timing does it's own level exits while it is enabled
    if (IL_exitState == IL_EXIT_NONE && (IL_previousGamestate == GAMEPLAY || IL_previousGamestate == INTERACTING) && (enteredMainLoad) && (!wasInHomeworld))
    {
        // BeginLevelLoad only prepares the loading state
        IL_exitResumeGamestate = gamestate;
        IL_exitType = IL_EXIT_PORTAL;
        IL_exitState = IL_EXIT_FADING;
        IL_retrySelected = true;
    }

    if (IL_exitState == IL_EXIT_NONE && (gamestate == GAMEPLAY || gamestate == INTERACTING))
    {
        IL_sourceLevelID = levelID;
        IL_sourceSubLevelID = subLevelID;
    }

    IL_previousGamestate = gamestate;
}

static void SaveILTimerAtCurrentFrame(void)
{
    IL_mainTimer.timer = globalTimer - IL_mainTimerAtReset;
    FramesToTimer(&IL_mainTimer);
    LoadAscii(&IL_mainTimer, IL_mainTimerAscii);
}

static void SaveBestTimeAtCurrentFrame(void)
{
    // Save your best level time so far, if you beat it, or it doesn't yet exist
    if (IL_mainTimer.timer < currentBestTimeInLevel.timer || currentBestTimeInLevel.timer == 0)
    {
        currentBestTimeInLevel.timer = IL_mainTimer.timer;
        FramesToTimer(&currentBestTimeInLevel);
        LoadAscii(&currentBestTimeInLevel, currentBestTimeInLevelAscii);
    }
}

static void FinishILExitFade(void)
{
    drawScreenBlack = 0xFF;
    SaveILTimerAtCurrentFrame();
    SaveBestTimeAtCurrentFrame();

    IL_exitState = IL_EXIT_PROMPT;
    IL_timerState = TIMER_DISPLAYING;
    IL_retrySelected = true;
    shouldSaveTimerPortal = false;

    // Freezing here holds the game after it rendered the fully-black frame but before that next update can issue a CD read for the levl load
    gamestate = FROZEN;
}

static void RetryILLevel(void)
{
    ILExitType exitType = IL_exitType;

    IL_exitState = IL_EXIT_NONE;
    IL_timerState = TIMER_RUNNING;
    IL_preparingToStartTimer = true;
    shouldSaveTimerPortal = false;

    // Reset the entire level if SBR, because it ends in a sub level, it must reload to main level
    if (currentLevel == SUPER_BONUS_ROUND)
    {
        printf_syscall("test\n");
        LoadLevel(3, SUPER_BONUS_ROUND);
        ClearCollectables();
        return;
    }

    if (exitType == IL_EXIT_PORTAL)
    {
        // Restore the levelid to fly to
        levelID = IL_sourceLevelID;
        subLevelID = IL_sourceSubLevelID;
        EndILPortalTransition(false);
    }

    gamestate = GAMEPLAY;
    RestartLevelFromBeginning();
}

static void ContinueILExit(void)
{
    ILExitType exitType = IL_exitType;
    s32 exitContext = IL_exitContext;

    IL_exitState = IL_EXIT_NONE;
    IL_timerState = TIMER_RUNNING;
    IL_preparingToStartTimer = false;
    shouldSaveTimerPortal = false;

    // If in a boss level, do a cutscene load to the next homeworld
    // If in SBR final sorc, load back to SBR main level
    if (currentLevel == SUPER_BONUS_ROUND)
    {
        //printf_syscall("B\n");
        LoadLevel(3, SUPER_BONUS_ROUND);
        ClearCollectables();
    }
    else if (isInBossLevel)
    {
        //printf_syscall("A\n");
        u32 level_to_warp_to = currentLevel + 3; // A boss is always X7, so the next homeworld would be X7 + 3
        level_to_warp_to = level_to_warp_to > 40 ? 40 : level_to_warp_to; // Clamp to a 40, so sorc doesn't take us to 50

        u32 splash_screen = (level_to_warp_to / 10) - 1; // hw splash screens are 0, 1, 2, 3. (If i'm really tight on code space, this isn't fully required, just nice to show the right splash screen. Remove if desperate for space!)

        LoadLevel(splash_screen, level_to_warp_to);
        ClearCollectables();
    }
    else if (exitType == IL_EXIT_PORTAL)
    {
        // Resume the regular load out of the portal
        EndILPortalTransition(true);
        ClearCollectables();
    }
    else // Resume the regular load out of the pause menu
    {
        BeginLevelLoad(exitContext);
        ClearCollectables();
        if (gamestate == LOADING_LEVEL)
        {
            framesInScenario = 0x20;
            drawScreenBlack = 0xFF;
        }
    }
}

static void UpdateILExitPrompt(void)
{
    if ((isButtonPressed & (LEFT_BUTTON | RIGHT_BUTTON)) != 0)
    {
        IL_retrySelected = !IL_retrySelected;
        PlaySound(10, 0, 0);
    }

    if ((isButtonPressed & CIRCLE_BUTTON) != 0)
    {
        ContinueILExit();
        PlaySound(7, 0, 0);
    }
    else if ((isButtonPressed & X_BUTTON) != 0)
    {
        if (IL_retrySelected)
        {
            RetryILLevel();
            PlaySound(7, 0, 0);
        }
        else
        {
            ContinueILExit();
            PlaySound(7, 0, 0);
        }
    }
}

//! Every Frame Update
void ILTimerUpdate(void)
{
    if (g_ILTimerMode > 0) // Is > 0, aka on, and only when in a level, not hw
    {

        DetectILPortalTransition();

        // When entering a new level, prepare the IL timer to start when you enter gameplay
        if (!IL_isLoadComboPressed)
        {
            if (gamestate == LOADING_CUTSCENE || gamestate == LOADING_LEVEL)
            {
                IL_preparingToStartTimer = true;
                IL_isLoadComboPressed = true;
            }
        }
        // When reload level hotkey is pressed, prepare the IL timer to start when you enter gameplay
        if ((isButtonHeld == RELOAD_LEVEL_HOTKEY) && !IL_isLoadComboPressed)
        {
            IL_preparingToStartTimer = true;
            IL_isLoadComboPressed = true;
            if (subLevelID != 0)
            {
                LoadLevel(3, currentLevel);
            }
        }
        // Start the IL timer when entering gameplay
        if (IL_preparingToStartTimer && (gamestate == GAMEPLAY || gamestate == INTERACTING))
        {
            //printf_syscall("Resetting timer!\n");
            IL_mainTimerAtReset = globalTimer;
            IL_timerState = TIMER_RUNNING;
            IL_preparingToStartTimer = false;

        }
        // Allow the hotkey to be pressed again if buttons released, or fully reset
        if ((isButtonHeld != RELOAD_LEVEL_HOTKEY || gamestate == GAMEPLAY) && IL_isLoadComboPressed)
        {
            IL_isLoadComboPressed = false;
        }

        if (HasBossFinished())
        {
            FinishILExitFade();
        }

        // if (gamestate == LOADING_LEVEL && IL_exitState == IL_EXIT_NONE)
        // {
        //     IL_mainTimerAtReset = globalTimer;
        //     IL_timerState = TIMER_RUNNING;
        // }

        // If has entered loading screen, reset best time
        if (gamestate == LOADING_CUTSCENE || gamestate == LOADING_LEVEL)
        {
            currentBestTimeInLevel.timer = 0;
        }

        if (IL_exitState == IL_EXIT_FADING)
        {
            if (UpdateILExitFade())
            {
                FinishILExitFade();
            }
        }
        else if (IL_exitState == IL_EXIT_PROMPT)
        {
            UpdateILExitPrompt();
        }

        // Calculate the current time while paused, during an exit fade, or on every frame with TIMER_SHOW_ALWAYS
        if (IL_timerState == TIMER_RUNNING)
        {
            if (IL_exitState == IL_EXIT_FADING ||
                g_ILTimerMode == TIMER_SHOW_ALWAYS)
            {
                SaveILTimerAtCurrentFrame();
            }
        }

        // Display the timer
        if ((g_ILTimerMode == TIMER_SHOW_ALWAYS || IL_timerState == TIMER_DISPLAYING) && (gamestate == GAMEPLAY || gamestate == PAUSED) && menu_frames_closed > 2)
        {
            int x1 = 415;
            int x2 = 504;
            int y1 = 204;
            int y2 = 224;
            int text_x_pos_adjust;

            if (IL_mainTimer.minutes == 0)
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
            DrawTextCentered(IL_mainTimerAscii, text_x_pos_adjust, 210, 2);
        }
    }

    if (IL_exitState == IL_EXIT_PROMPT)
    {
        ILTimerFinishedUpdate();
    }
}



u32 xx1 = 110;
u32 xx2 = 400;
u32 yy1 = 70;
u32 yy2 = 175;
// Draw Finished Screen

u32 test_color = 0;
void ILTimerFinishedUpdate(void)
{
    static u32 blink_timer = 0;
    blink_timer = ((blink_timer + 1) % 20);

    // Highlight selected option
    u32 yes_color = IL_retrySelected ? COLOR_BRIGHT_YELLOW : COLOR_YELLOW;
    u32 no_color = IL_retrySelected ? COLOR_YELLOW : COLOR_BRIGHT_YELLOW;

    // Blink highlighted option
    if (yes_color == COLOR_BRIGHT_YELLOW)
    {
        yes_color = (blink_timer < 10) ? COLOR_BRIGHT_YELLOW : COLOR_YELLOW;
    }
    else if (no_color == COLOR_BRIGHT_YELLOW)
    {
        no_color = (blink_timer < 10) ? COLOR_BRIGHT_YELLOW : COLOR_YELLOW;
    }

    DrawTextbox(xx1, xx2, yy1, yy2);

    // Change header text and your time color if is best time
    u32 your_time_color = COLOR_ORANGE;
    if (currentBestTimeInLevel.timer == IL_mainTimer.timer)
    {
        DrawTextCentered("New Best Time!", ((xx2 + xx1) / 2), ((yy1 + yy2) / 2) - 45, COLOR_YELLOW);
        your_time_color = COLOR_GREEN;
    }
    else
    {
        DrawTextCentered("IL Complete!", ((xx2 + xx1) / 2), ((yy1 + yy2) / 2) - 45, COLOR_YELLOW);
        your_time_color = COLOR_BROWN;
    }


    DrawTextCentered("Your Time:", ((xx2 + xx1) / 2) - 90, ((yy1 + yy2) / 2) - 20, COLOR_YELLOW);
    DrawTextCentered("Best Time:", ((xx2 + xx1) / 2) + 90, ((yy1 + yy2) / 2) - 20, COLOR_YELLOW);

    // Show current, and best time
    DrawTextCentered(IL_mainTimerAscii, ((xx2 + xx1) / 2) - 90, ((yy1 + yy2) / 2) - 5, your_time_color);
    DrawTextCentered(currentBestTimeInLevelAscii, ((xx2 + xx1) / 2) + 90, ((yy1 + yy2) / 2) - 5, COLOR_GREEN);

    DrawTextCentered("Try Again?", ((xx2 + xx1) / 2), ((yy1 + yy2) / 2) + 25, 2);
    DrawTextCentered("Yes", ((xx2 + xx1) / 2) - 50, ((yy1 + yy2) / 2) + 40, yes_color);
    DrawTextCentered("No",((xx2 + xx1) / 2) + 50, ((yy1 + yy2) / 2) + 40, no_color);
}
