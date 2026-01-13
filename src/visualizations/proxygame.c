#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include <syscalls.h>
#include <gamestates.h>
#include <primitives.h>
#include <gamestates.h>

byte *sunnyFirstEnemyPtr = 0x80191480;
int proxyValue = 0;
bool reset = true;
int scaledScore = 0;

int scale_score(raw_score, max_proxy_height)
{
    const int min_proxy_height = 4000;
    const int RANGE = max_proxy_height - min_proxy_height;

    const RANGE_MAX = 100; // the range should be 1-100
    const int SCALED_UNIT = RANGE / 100;

    int score_adjusted = raw_score - min_proxy_height; // Give the score within the range
    int score_scaled = score_adjusted / SCALED_UNIT;   // Give the adjusted score between 0-100

    return score_scaled;
}

void ProxyGame(void)
{
    if (gamestate == GAMEPLAY)
    {
        DrawShadedLine(30, 60, 30, 160);
        DrawShadedLine(28, 60, 28, 160); // yucky need to make own line func

        DrawShadedLine(23, 160, 35, 160);
        DrawShadedLine(23, 60, 35, 60);

        if (appliedMomentumVertical > 4000 && reset)
        {
            printf_syscall("%d \n", appliedMomentumVertical);
            proxyValue = appliedMomentumVertical;
            reset = false;
        }
        if (proxyValue != 0)
        {
            ONCE scaledScore = scale_score(proxyValue, 17888);
            DrawText("X", 25, 160 - scaledScore, 0x4, 0);

            if (isButtonPressed == CIRCLE_BUTTON)
            {
                reset = true;
                proxyValue = 0;
            }
        }
    }
}