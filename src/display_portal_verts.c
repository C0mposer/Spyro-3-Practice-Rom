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

// Render-distance cull. Skip trigger triangles whose forward (view-space) depth
// exceeds this, to cut GPU overdraw when many large trigger polys are visible.
// Lower this if the game still hitches; raise it if portals you want disappear.
// The OT can hold roughly up to ~0x2A000 of depth before it stops bucketing.
#define PORTAL_MAX_RENDER_DEPTH 0x5555

#define PRIMITIVE_BUFFER_MARGIN 0x400
// TEMP: opaque for testing. 0x20 = flat opaque tri. Transparent was 0x22FF00FF.
#define PORTAL_POLYGON_COLOR 0x20FF00FF

// Camera world position (camera.nextCameraPosCartesian / cameraY / cameraZ).
// DrawPortalRelated transforms each vertex relative to these.
#define CAMERA_POS_X (*(volatile s32*)0x8006E020)
#define CAMERA_POS_Y (*(volatile s32*)0x8006E024)
#define CAMERA_POS_Z (*(volatile s32*)0x8006E028)

// DrawPortalRelated loads (worldVertex - camera) >> shift into the GTE's 16-bit
// vertex registers. Keep the shifted magnitude comfortably under 0x7FFF so it
// never wraps (which makes the portal repeat in world space).
#define GTE_INPUT_SAFE_LIMIT 0x7000
#define MAX_TRANSFORM_SHIFT 15

// Semi-transparency rate (ABR) for the portal polygon. For an untextured
// POLY_F3 the GPU takes the blend rate from the current texture-page register,
// i.e. leftover global state from the previous OT primitive -> shimmering. We
// pin it with our own draw-mode packet. 0 = B/2+F/2 (standard translucent),
// 1 = B+F (additive glow), 2 = B-F, 3 = B+F/4.
#define PORTAL_BLEND_ABR 0
#define PORTAL_DRAW_MODE_WORD (0xE1000000u | (((u32)PORTAL_BLEND_ABR & 3u) << 5))

typedef struct
{
    u32 tag;
    u32 color_and_command;
    u32 xy0;
    u32 xy1;
    u32 xy2;
} PolyF3;

// GP0(E1h) "Draw Mode / Texture Page" packet: 1 tag word + 1 command word.
typedef struct
{
    u32 tag;
    u32 code;
} DrawModePrim;

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

    if (collision == 0 || collision->surface_types == 0 ||
        ptr_levelRelated == 0)
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

static u32 PackScreenPoint(const Vec3* point)
{
    return (u16)point->x | ((u32)(u16)point->y << 16);
}

// Pick the smallest right-shift that keeps every vertex of the triangle within
// the GTE's signed 16-bit vertex range once made camera-relative. Screen X/Y
// are invariant to this shift (perspective divide cancels it); only depth
// precision changes. This is what stops the portals repeating in world space.
static u32 PickTransformShift(const Triangle* world)
{
    const Vec3* verts = &world->v1; // v1, v2, v3 are contiguous
    s32 cameraX = CAMERA_POS_X;
    s32 cameraY = CAMERA_POS_Y;
    s32 cameraZ = CAMERA_POS_Z;
    s32 maxComponent = 0;
    u32 shift = 0;
    s32 i;

    for (i = 0; i < 3; i++)
    {
        s32 dx = verts[i].x - cameraX;
        s32 dy = verts[i].y - cameraY;
        s32 dz = verts[i].z - cameraZ;

        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        if (dz < 0) dz = -dz;

        if (dx > maxComponent) maxComponent = dx;
        if (dy > maxComponent) maxComponent = dy;
        if (dz > maxComponent) maxComponent = dz;
    }

    while ((maxComponent >> shift) > GTE_INPUT_SAFE_LIMIT && shift < MAX_TRANSFORM_SHIFT)
    {
        shift++;
    }

    return shift;
}

static s32 GetWorldDepthBucket(s32 depth)
{
    s32 bucket = depth >> 7;

    if (bucket >= WORLD_OT_SPLIT_DEPTH)
    {
        bucket += WORLD_OT_SPLIT_OFFSET;
    }

    return bucket + WORLD_OT_BASE_OFFSET;
}

static bool DrawPortalTriangle(s32 triangle_index)
{
    Triangle world;
    Triangle screen;
    /* TEMP(transparency disabled): DrawModePrim* draw_mode; */
    PolyF3* primitive;
    s32 depth;
    s32 bucket;
    u32 shift;

    UnpackCollisionTriangle(triangle_index, &world);
    shift = PickTransformShift(&world);
    DrawPortalRelated(&screen.v1, &world.v1, shift);
    DrawPortalRelated(&screen.v2, &world.v2, shift);
    DrawPortalRelated(&screen.v3, &world.v3, shift);

    if (screen.v1.z < NEAR_CLIP_DEPTH ||
        screen.v2.z < NEAR_CLIP_DEPTH ||
        screen.v3.z < NEAR_CLIP_DEPTH)
    {
        return true;
    }

    if ((MCP_CalcScreenClipFlags(screen.v1.x, screen.v1.y, screen.v1.z) &
        MCP_CalcScreenClipFlags(screen.v2.x, screen.v2.y, screen.v2.z) &
        MCP_CalcScreenClipFlags(screen.v3.x, screen.v3.y, screen.v3.z)) != 0)
    {
        return true;
    }

    depth = (screen.v1.z + screen.v2.z + screen.v3.z) / 3;

    // Render-distance cull: drop far triggers to reduce GPU overdraw.
    if (depth > PORTAL_MAX_RENDER_DEPTH)
    {
        return true;
    }

    bucket = GetWorldDepthBucket(depth);
    if (bucket < 0 || bucket >= WORLD_OT_BUCKET_COUNT)
    {
        return true;
    }

    primitive = (PolyF3*)ptr_primitiveStruct;
    if ((u8*)(primitive + 1) > (u8*)primitiveBufferEnd - PRIMITIVE_BUFFER_MARGIN)
    {
        return false;
    }

    // TEMP(transparency disabled): drawing opaque, so the semi-transparency
    // draw-mode packet is skipped. Re-enable this block (and add draw_mode back
    // to the reservation above) to restore translucent portals.
    // draw_mode = (DrawModePrim*)ptr_primitiveStruct;
    // primitive = (PolyF3*)(draw_mode + 1);
    // draw_mode->tag = 0x01000000; // 1 word follows
    // draw_mode->code = PORTAL_DRAW_MODE_WORD;

    primitive->tag = 0x04000000;
    primitive->color_and_command = PORTAL_POLYGON_COLOR;
    primitive->xy0 = PackScreenPoint(&screen.v1);
    primitive->xy1 = PackScreenPoint(&screen.v2);
    primitive->xy2 = PackScreenPoint(&screen.v3);

    // TEMP(transparency disabled): AddToWorldTable(draw_mode, bucket);
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

void DrawPortalsAndPortalCollision(void)
{
    MCP_DrawPortals();
    DrawPortalPolygons();
}
