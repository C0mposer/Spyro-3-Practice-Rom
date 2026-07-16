#ifndef COLLISION_H
#define COLLISION_H

#include <types.h>
#include <vector.h>

typedef struct
{
    Vec3 v1;
    Vec3 v2;
    Vec3 v3;
} Triangle;

typedef struct
{
    u32 x;
    u32 y;
    u32 z;
} CollisionTri;

typedef struct
{
    u32 tri_count;
    u32 flagged_count;
    u32 unknown_08;
    u16* block_tree;
    u16* blocks;
    CollisionTri* triangles;
    u8* surface_types;
    u32 unknown_1c;
    u32 unknown_20;
} CollisionHeader;

typedef struct
{
    u8 type;
    u8 padding[3];
    s32 parameter;
} TriggerRecord;

#endif /* COLLISION_H */
