#include <symbols.h>
#include <buttons.h>
#include <difficulty.h>
#include <gamestates.h>

s32 newDifficulty = DIFFICULTY_MEDIUM;
bool difficultyJustChanged = false;
u32 displayTimer = 0;

bool isButtonsCurrentlyHeld = true;