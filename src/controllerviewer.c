#include <gamestates.h>
#include <buttons.h>
#include <symbols.h>

void ControllerViewer(void)
{

    if (gamestate == GAMEPLAY)
    {
        DrawTextbox(10,170,170,225);
        DrawText("I am GEEKING 12345 ********",100,100,0x5,0);
        if (analogEnabled)
        {
            DrawShadedLine(50,203,(50+((leftAnalogX-0x80)/7)),(203+((leftAnalogY-0x80)/11)));
        }

        if (isButtonHeld & X_BUTTON)
        {
            DrawText("<",120,211,0x1,0);
        }

        if (isButtonHeld & SQUARE_BUTTON)
        {
            DrawText(">",102,199,0x1,0);
        }
        if (isButtonHeld & TRIANGLE_BUTTON)
        {
            DrawText("}",120,187,0x1,0);
        }
        if (isButtonHeld & CIRCLE_BUTTON)
        {
            DrawText("{",138,199,0x1,0);
        }
    }
    

}
