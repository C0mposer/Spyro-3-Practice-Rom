#pragma once

#include <types.h>

// When a checkpoint is loaded, it can set memory values from its CheckpointMemoryPatch
typedef enum PatchSize
{
    PATCH_1BYTE,
    PATCH_2BYTE,
    PATCH_4BYTE
}PatchSize;

typedef struct CheckpointMemoryPatch
{
    u8 patchSize; // It uses the patch size enum, but im making it a u8 instead of the enum type, because enums are 32bit by default
    u8 address_no_prefix[3]; // No 0x80. (0x80123456 -> 0x123456)
    u32 patchValue; // Value to patch at the address
}CheckpointMemoryPatch;
