#ifndef CONTROLLERVIEWER_H
#define CONTROLLERVIEWER_H
#include <gamestates.h>


void ControllerViewer(void)
{

    if (gamestate == gameplay)
    {
        DrawTextboxSquare(10,115,185,220);
        if (analogEnabled)
        {
            DrawShadedLine(35,200,(35+((leftAnalogX-0x80)/8)),(200+((leftAnalogY-0x80)/13)));
        }
        
    }
    

}

#endif