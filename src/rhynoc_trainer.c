#include <buttons.h>
#include <gamestates.h>
#include <level_ids.h>
#include <symbols.h>
#include <syscalls.h>
#include <types.h>

enum InputSequence
{
    INITIAL_CHARGE_JUMP,
    GLIDE,
    CHARGE
};
#define RHYNOC_THROWN_ANIM 4

const raw_memory_ptr* rhynocAnimState = 0x8018E414;
// const raw_memory_ptr* rhynocAnimKeyFrame = 0x8018E417;
u32 input_timer_during_thrown_anim = 0;

u32 rhynoc_currentButtonSequence = 0;

u32 rhynoc_input_sequence[3] = {0};

u32 rhynoc_wait_to_show = 0;

#define INPUT_SEQUENCE_FINISHED 3
extern bool g_rhynocTrainerEnabled;
extern bool g_ILTimerMode;
void RhynocProxyTrainer()
{
    if (currentLevel == FIREWORKS_FACTORY && g_rhynocTrainerEnabled)
    {
        if (g_ILTimerMode == 0) // Only allow when IL mode is off
        {
            if (fade_effect != 0)
            {
                // For loop? Never heard of her
                rhynoc_input_sequence[0] = 0;
                rhynoc_input_sequence[1] = 0;
                rhynoc_input_sequence[2] = 0;
                rhynoc_currentButtonSequence = 0;
                rhynoc_wait_to_show = 0;
            }

            if (gamestate == GAMEPLAY && *rhynocAnimState == RHYNOC_THROWN_ANIM)
            {
                input_timer_during_thrown_anim++;
            }
            else if (*rhynocAnimState != RHYNOC_THROWN_ANIM)
            {
                input_timer_during_thrown_anim = 0;
            }

            if (*rhynocAnimState == RHYNOC_THROWN_ANIM && isButtonHeld == SQUARE_BUTTON + X_BUTTON && rhynoc_currentButtonSequence == 0)
            {
                rhynoc_input_sequence[INITIAL_CHARGE_JUMP] = input_timer_during_thrown_anim;
                rhynoc_currentButtonSequence++;
            }
            else if (*rhynocAnimState == RHYNOC_THROWN_ANIM && isButtonPressed & X_BUTTON && rhynoc_currentButtonSequence == 1)
            {
                rhynoc_input_sequence[GLIDE] = input_timer_during_thrown_anim;
                rhynoc_currentButtonSequence++;
            }
            else if (*rhynocAnimState == RHYNOC_THROWN_ANIM && isButtonPressed & SQUARE_BUTTON && rhynoc_currentButtonSequence == 2)
            {
                rhynoc_input_sequence[CHARGE] = input_timer_during_thrown_anim;
                rhynoc_currentButtonSequence++;
            }
            else if (rhynoc_currentButtonSequence == INPUT_SEQUENCE_FINISHED)
            {
                // printf_syscall("Charge Jump: %d, Glide: %d, Proxy: %d\n",
                // rhynoc_input_sequence[0], rhynoc_input_sequence[1], rhynoc_input_sequence[2]);

                if (rhynoc_wait_to_show < 5)
                {
                    rhynoc_wait_to_show++;
                }

                else
                {
                    char* buffer[16];
                    char* buffer2[16];
                    char* buffer3[16];
                    sprintf(buffer, "Jump: %d", rhynoc_input_sequence[0]);
                    sprintf(buffer2, "Glide: %d", rhynoc_input_sequence[1]);
                    sprintf(buffer3, "Proxy: %d", rhynoc_input_sequence[2]);

                    DrawText(buffer, 10, 60, 1, 0);
                    DrawText(buffer2, 10, 70, 1, 0);
                    DrawText(buffer3, 10, 80, 1, 0);
                }
            }
        }
    }
}
