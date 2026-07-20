#include "menu/menu.h"
#include <symbols.h>
#include <types.h>

// Because DrawPortalPolygons is loaded from the CD, we can't have this function
// in the file that gets loaded from the CD
extern bool hasLoadedCDCode;
void DrawPortalsAndPortalCollision(void)
{
    DrawPortals();
    #ifndef INJECTION_ONLY
    if (hasLoadedCDCode)
    {
        #endif
        DrawPortalPolygons();
        #ifndef INJECTION_ONLY
    }
    #endif
}
