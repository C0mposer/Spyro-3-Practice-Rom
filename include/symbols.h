#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <types.h>
#include <vector.h>

extern s32 spyroX;
extern s32 spyroY;
extern s32 spyroZ;
extern s32 spyroAngle;

extern u32 gamestate;

extern u32 globalGems;
extern u32 globalEggs;
extern u32 lives;

extern u8 gemsCollectedFlags[0x500];
extern u8 eggsCollectedFlags[0xF8];
extern u8 progressFlags[0x4C];
extern u8 skillPointFlags[0x14];

extern u32 isButtonHeld;
extern u32 isButtonPressed;

extern bool analogEnabled;
extern u8 leftAnalogX;
extern u8 leftAnalogY;

extern s16 lowLevelMusicVolume;

extern u8 titleScreenState;
extern s32 globalTimer;
extern s32 * sunriseUpdateParticlePtr;


void* memset(void* dest, u8 value, u32 size);

void DrawTextboxSquare(s16 x1, s16 x2, s16 y1, s16 y2);
extern void DrawTextbox(s32 x1, s32 x2, s32 y1, s32 y2);
extern void DrawShadedLine(s16 x1, s16 y1, s16 x2, s16 y2);
void DrawTextCentered(const char* text, s32 x, s32 y, s32 color);
void DrawScreenColour(s32 unk, u8 R, u8 G, u8 B);

// \* is star > is square
extern void DrawText(const char *text,s32 x,s16 y,s32 colour,s32 *unkptr);

s32 * DropGem(s32 object, uint param_2, s32 vec1, s32 vec2);
s32 PlaySound(s32 soundID, s32 param_2,u8 param_3);

void SetPrimitiveDrawMode(s32 param_1);

void LoadLevel(s32 mo, s32 cl);

void test();
char * SunriseUpdateParticles(s32 param_1, s32 param_2, Vec3 *param_3, Vec3 *param_4);



#endif //SYMBOLS_H

    