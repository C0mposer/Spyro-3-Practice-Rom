#ifndef CONTROLLERVIEWER_H
#define CONTROLLERVIEWER_H
#include <gamestates.h>
#include <buttons.h>

void ControllerViewer(void)
{

    if (gamestate == gameplay)
    {
        DrawTextbox(10,115,185,220);
        if (analogEnabled)
        {
            DrawShadedLine(35,205,(35+((leftAnalogX-0x80)/8)),(205+((leftAnalogY-0x80)/13)));
        }
        if (isButtonHeld | ~X_BUTTON)
        {
            DrawText("<",75,210,0x1,0);
        }
        
    }
    

}

#endif