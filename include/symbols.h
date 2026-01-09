#ifndef SYMBOLS_H
#define SYMBOLS_H
#include <types.h>
#define SCREEN_LEFT_EDGE 0x0
#define SCREEN_RIGHT_EDGE 0x200
#define SCREEN_TOP_EDGE 0x0
#define SCREEN_BOTTOM_EDGE 0xE8


extern int spyroX;
extern int spyroY;
extern int spyroZ;
extern int spyroAngle;

extern int isButtonHeld;
extern int isButtonPressed;
extern int gamestate;
extern short musicVolume;


extern bool analogEnabled;
extern byte leftAnalogX;
extern byte leftAnalogY;

extern byte titleScreenState;
extern int globalTimer;



void DrawTextboxSquare(short x1,short x2,short y1,short y2);
extern void DrawTextbox(int x1,int x2,int y1,int y2);
extern void DrawShadedLine(short x1,short y1,short x2,short y2);
void DrawTextCentered(const char* text, int x, int y, int color);
void DrawScreenColour(int unk, byte R, byte G, byte B);

// \* is star > is square
extern void DrawText(const char *text,int x,short y,int colour,int *unkptr);

int * DropGem(int object,uint param_2,int vec1,int vec2);
int PlaySound(int soundID, int param_2,byte param_3);

void SetPrimitiveDrawMode(int param_1);

void LoadLevel(int mo, int cl);

void test();



#endif //SYMBOLS_H

    