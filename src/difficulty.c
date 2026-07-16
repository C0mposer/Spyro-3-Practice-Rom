#include <symbols.h>
#include <buttons.h>
#include <difficulty.h>
#include <gamestates.h>

char* difficultyStringLUT[3] = { "Easy", "Normal", "Hard" }; // Renamed Medium to Normal for now. Conflicted on what to call it because most people only talk about "easy & hard", but really the difficulty you start at is not easy.

s32 newDifficulty = DIFFICULTY_MEDIUM;
bool difficultyJustChanged = false;
u32 displayTimer = 0;

bool isButtonsCurrentlyHeld = true;

void SetDifficulty(s32 difficulty)
{
    s32 score;

    if (difficulty == DIFFICULTY_EASY)
        score = 30;
    else if (difficulty == DIFFICULTY_HARD)
        score = -40;
    else // Medium
        score = 0;

    adaptiveDifficultyScore = score;
    baseDifficulty = difficulty;
    currentDifficulty = difficulty;
}

// // Change difficulty with hotkey
// void DifficultyUpdate()
// {
//     if (isButtonHeld == L2_BUTTON + L1_BUTTON + UP_BUTTON && isButtonsCurrentlyHeld == false)
//     {
//         newDifficulty++;

//         if (newDifficulty > 2) { newDifficulty = DIFFICULTY_HARD; }

//         //SetDifficulty(newDifficulty); <- Was setting only once, now doing every frame. Keeping comment just incase I find a way to not have to do it every frame

//         difficultyJustChanged = true;
//         isButtonsCurrentlyHeld = true;
//     }
//     else if (isButtonHeld == L2_BUTTON + L1_BUTTON + DOWN_BUTTON && isButtonsCurrentlyHeld == false)
//     {
//         newDifficulty--;

//         if (newDifficulty < 0) { newDifficulty = DIFFICULTY_EASY; }

//         //SetDifficulty(newDifficulty); <- Was setting only once, now doing every frame. Keeping comment just incase I find a way to not have to do it every frame

//         difficultyJustChanged = true;
//         isButtonsCurrentlyHeld = true;

//     }
//     else if (isButtonHeld != L2_BUTTON + L1_BUTTON + DOWN_BUTTON && isButtonHeld != L2_BUTTON + L1_BUTTON + UP_BUTTON)
//     {
//         isButtonsCurrentlyHeld = false;
//     }

//     SetDifficulty(newDifficulty); // Set every frame to avoid being updated by the game
//     DisplayDifficultyChange();
// }

// // Display new difficulty for 1s
// void DisplayDifficultyChange()
// {
//     if (difficultyJustChanged == true)
//     {
//         displayTimer = 30;
//         difficultyJustChanged = false;
//     }

//     if (displayTimer > 0 && gamestate == GAMEPLAY)
//     {
//         char* difficultyString = difficultyStringLUT[newDifficulty];
//         char buf[20] = { 0 };
//         sprintf(buf, "Difficulty: %s", difficultyString);
//         DrawText(buf, 10, 215, 0x1, 0);

//         displayTimer--;
//     }
// }
