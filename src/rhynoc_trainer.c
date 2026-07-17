#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include <buttons.h>
#include <level_ids.h>


enum InputSequence
{
    INITIAL_CHARGE_JUMP,
    GLIDE,
    CHARGE
};
#define RHYNOC_THROWN_ANIM 4

const raw_memory_ptr* rhynocAnimState = 0x8018E414;
const raw_memory_ptr* rhynocAnimKeyFrame = 0x8018E417;

u32 currentButtonSequence = 0;

u16 input_sequence[3] = { 0 };
#define INPUT_SEQUENCE_FINISHED 3

u32 wait_to_show = 0;

extern bool g_rhynocTrainerEnabled;
void RhynocProxyTrainer()
{
    if (currentLevel == FIREWORKS_FACTORY && g_rhynocTrainerEnabled)
    {
        if (fade_effect != 0)
        {
            // For loop? Never heard of her
            input_sequence[0] = 0;
            input_sequence[1] = 0;
            input_sequence[2] = 0;
            currentButtonSequence = 0;
            wait_to_show = 0;
        }

        //printf_syscall("*rhynocAnimState %d, currentButtonSequence %d , input_sequence[0] %d, input_sequence[1] %d, input_sequence[2] %d\n", *rhynocAnimState, currentButtonSequence, input_sequence[0], input_sequence[1], input_sequence[2]);
        if (*rhynocAnimState == RHYNOC_THROWN_ANIM && isButtonHeld == SQUARE_BUTTON + X_BUTTON && currentButtonSequence == 0)
        {
            input_sequence[INITIAL_CHARGE_JUMP] = *rhynocAnimKeyFrame;
            currentButtonSequence++;
        }
        else if (*rhynocAnimState == RHYNOC_THROWN_ANIM && isButtonPressed == X_BUTTON && currentButtonSequence == 1)
        {
            input_sequence[GLIDE] = *rhynocAnimKeyFrame;
            currentButtonSequence++;
        }
        else if (*rhynocAnimState == RHYNOC_THROWN_ANIM && isButtonPressed == SQUARE_BUTTON && currentButtonSequence == 2)
        {
            input_sequence[CHARGE] = *rhynocAnimKeyFrame;
            currentButtonSequence++;
        }
        else if (currentButtonSequence == INPUT_SEQUENCE_FINISHED)
        {
            //printf_syscall("Charge Jump: %d, Glide: %d, Proxy: %d\n", input_sequence[0], input_sequence[1], input_sequence[2]);

            if (wait_to_show < 5)
            {
                wait_to_show++;
            }

            else
            {
                char* buffer[16];
                char* buffer2[16];
                char* buffer3[16];
                sprintf(buffer, "Charge Jump: %d", input_sequence[0]);
                sprintf(buffer2, "Glide: %d", input_sequence[1]);
                sprintf(buffer3, "Proxy/Charge: %d", input_sequence[2]);

                DrawText(buffer, 10, 60, 1, 0);
                DrawText(buffer2, 10, 70, 1, 0);
                DrawText(buffer3, 10, 80, 1, 0);
            }
        }
    }
}

