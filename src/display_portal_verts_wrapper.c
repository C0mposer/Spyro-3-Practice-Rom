#include <types.h>
#include <symbols.h>
#include "menu/menu.h"

// Because DrawPortalPolygons is loaded from the CD, we can't have this function in the file that gets loaded from the CD
extern bool hasLoadedCDCode;
void DrawPortalsAndPortalCollision(void)
{
    MCP_DrawPortals();
    if (hasLoadedCDCode)
    {
        DrawPortalPolygons();
    }
}