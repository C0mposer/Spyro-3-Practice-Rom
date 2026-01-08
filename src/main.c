#include <types.h>
#include <symbols.h>
#include "menu/menu.h"
#include <buttons.h>
#include <syscalls.h>


int test = 83;

void ModMain(void) {
vec3 myvec = {spyroX,spyroY,spyroZ};
vec3 myvec2 = {spyroX+400,spyroY+400,spyroZ+400};
if (isButtonPressed == TRIANGLE_BUTTON)
{
    test++;
}
SunriseUpdateParticles(0x9,41,&myvec,0);

printf_syscall("%d \n", test);


UpdateAllMenus();
}