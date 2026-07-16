#include <types.h>
#include <syscalls.h>
#include <symbols.h>
#include <buttons.h>
#include <gamestates.h>
#include <upgrades.h>
#include <hotkeys.h>
#include "menu/menu.h"

Vec3 savedStartingPosition = { 0 };
Vec3 savedStartingPositionSubLevel = { 0 };
u32 savedStartingAngle = 0;
u32 savedStartingAngleSubLevel = 0;
bool speedUpResetPending = false;
bool hasSavedSpyroPosition = false;

Vec3 savedSpyroPosition = { 0 };
s32 savedSpyroYawAngle = 0;
u32 savedPositionLevelID = 0;
u32 savedPositionSubLevelID = 0;

u32 reloadSpyroTimer = 0;