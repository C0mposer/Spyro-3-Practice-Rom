#include <types.h>
#include <symbols.h>
#include "menu/menu.h"

#define PORTAL_TRIGGER_TYPE 6
#define NO_TRIGGER_INDEX 0x3F
#define MAX_PORTAL_TRIANGLES 64

#define NEAR_CLIP_DEPTH 0x100
#define WORLD_OT_SPLIT_DEPTH 0x108
#define WORLD_OT_SPLIT_OFFSET 0x28
#define WORLD_OT_BASE_OFFSET 3
#define WORLD_OT_BUCKET_COUNT 0x580

// Render-distance cull: skip trigger triangles whose forward (view-space) depth
// exceeds this, to cut GPU overdraw. Lower if the game hitches.
#define PORTAL_MAX_RENDER_DEPTH 0x5555

#define PRIMITIVE_BUFFER_MARGIN 0x400
#define PORTAL_POLYGON_COLOR 0x20FF00FF // 0x20 = flat opaque tri (0x22 = transparent)

// Fixed scale-shift for DrawPortalRelated: it loads (worldVertex - camera) >> shift
// into the GTE's 16-bit vertex registers. 3 matches the game's own far-portal shift
// and keeps camera-relative coords in range out to ~0x40000 units -- well past the
// render-distance cull -- so portals don't repeat in world space. Bump to 4 if a
// very distant under-map trigger ever ghosts.
#define PORTAL_TRANSFORM_SHIFT 3

typedef struct
{
    u32 tag;
    u32 color_and_command;
    u32 xy0;
    u32 xy1;
    u32 xy2;
} PolyF3;

static CollisionHeader* cached_collision;
static u32 cached_level = (u32)-1;
static u32 cached_sublevel = (u32)-1;
static u32 portal_triangles[MAX_PORTAL_TRIANGLES];
static u8 portal_triangle_count;

static void CachePortalTriangles(void)
{
    CollisionHeader* collision = TerrainCollisionData;
    u32 triangle_count;
    u32 i;

    cached_collision = collision;
    cached_level = levelID;
    cached_sublevel = subLevelID;
    portal_triangle_count = 0;

    if (collision == 0 || collision->surface_types == 0 || ptr_levelRelated == 0)
    {
        return;
    }

    triangle_count = collision->flagged_count;
    if (triangle_count > collision->tri_count)
    {
        triangle_count = collision->tri_count;
    }

    for (i = 0; i < triangle_count; i++)
    {
        u8 trigger_index = collision->surface_types[i] & NO_TRIGGER_INDEX;
        TriggerRecord* trigger;

        if (trigger_index == NO_TRIGGER_INDEX)
        {
            continue;
        }

        trigger = ptr_levelRelated[trigger_index];
        if (trigger != 0 && trigger->type == PORTAL_TRIGGER_TYPE)
        {
            portal_triangles[portal_triangle_count++] = (u16)i;
            if (portal_triangle_count == MAX_PORTAL_TRIANGLES)
            {
                return;
            }
        }
    }
}

static bool DrawPortalTriangle(s32 triangle_index)
{
    Triangle world;
    Triangle screen;
    PolyF3* primitive;
    s32 depth;
    s32 bucket;

    UnpackCollisionTriangle(triangle_index, &world);
    DrawPortalRelated(&screen.v1, &world.v1, PORTAL_TRANSFORM_SHIFT);
    DrawPortalRelated(&screen.v2, &world.v2, PORTAL_TRANSFORM_SHIFT);
    DrawPortalRelated(&screen.v3, &world.v3, PORTAL_TRANSFORM_SHIFT);

    if (screen.v1.z < NEAR_CLIP_DEPTH ||
        screen.v2.z < NEAR_CLIP_DEPTH ||
        screen.v3.z < NEAR_CLIP_DEPTH)
    {
        return true;
    }

    if ((CalcScreenClipFlags(screen.v1.x, screen.v1.y, screen.v1.z) &
        CalcScreenClipFlags(screen.v2.x, screen.v2.y, screen.v2.z) &
        CalcScreenClipFlags(screen.v3.x, screen.v3.y, screen.v3.z)) != 0)
    {
        return true;
    }

    // depth is always >= NEAR_CLIP_DEPTH here, so bucket is always >= 0.
    depth = (screen.v1.z + screen.v2.z + screen.v3.z) / 3;
    if (depth > PORTAL_MAX_RENDER_DEPTH)
    {
        return true;
    }

    bucket = depth >> 7;
    if (bucket >= WORLD_OT_SPLIT_DEPTH)
    {
        bucket += WORLD_OT_SPLIT_OFFSET;
    }
    bucket += WORLD_OT_BASE_OFFSET;
    if (bucket >= WORLD_OT_BUCKET_COUNT)
    {
        return true;
    }

    primitive = (PolyF3*)ptr_primitiveStruct;
    if ((u8*)(primitive + 1) > (u8*)primitiveBufferEnd - PRIMITIVE_BUFFER_MARGIN)
    {
        return false;
    }

    primitive->tag = 0x04000000;
    primitive->color_and_command = PORTAL_POLYGON_COLOR;
    primitive->xy0 = (u16)screen.v1.x | ((u32)(u16)screen.v1.y << 16);
    primitive->xy1 = (u16)screen.v2.x | ((u32)(u16)screen.v2.y << 16);
    primitive->xy2 = (u16)screen.v3.x | ((u32)(u16)screen.v3.y << 16);

    AddToWorldTable(primitive, bucket);
    ptr_primitiveStruct = (u32*)(primitive + 1);
    return true;
}

void DrawPortalPolygons(void)
{
    u8 i;

    if (!main_menu.elements[DRAW_PORTAL_POLYGONS_TOGGLE].enabled)
    {
        return;
    }

    if (cached_collision != TerrainCollisionData ||
        cached_level != levelID || cached_sublevel != subLevelID)
    {
        CachePortalTriangles();
    }

    for (i = 0; i < portal_triangle_count; i++)
    {
        if (!DrawPortalTriangle(portal_triangles[i]))
        {
            return;
        }
    }
}

// extern bool hasLoadedCDCode;
// void DrawPortalsAndPortalCollision(void)
// {
//     DrawPortals();
//     if (hasLoadedCDCode)
//     {
//         DrawPortalPolygons();
//     }
// }
