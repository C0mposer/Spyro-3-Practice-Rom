// Common includes
#include <types.h>
#include <symbols.h>
#include <buttons.h>
#include <syscalls.h>

// Common constants
#define SCREEN_LEFT_EDGE 0x0
#define SCREEN_RIGHT_EDGE 0x200
#define SCREEN_TOP_EDGE 0x0
#define SCREEN_BOTTOM_EDGE 0xE8

#define isInHomeworld (currentLevel % 10 == 0)
#define wasInHomeworld (previousLevelID % 10 == 0)
