#include <types.h>
#include <symbols.h>

#define RENDER_BASED_ON_FLAGS_ADDR 0x8001E460

#define RENDER_BASED_ON_FLAGS_OG_OPCODE 0x27BDFFD8
#define JR_RA 0x03E00008
#define NOP 0x0000000

void DisableRendering()
{
    *((u32*)RENDER_BASED_ON_FLAGS_ADDR) = JR_RA;
    *((u32*)RENDER_BASED_ON_FLAGS_ADDR + 1) = NOP;
}

void EnableRendering()
{
    *((u32*)RENDER_BASED_ON_FLAGS_ADDR) = 0x27BDFFD8;
    *((u32*)RENDER_BASED_ON_FLAGS_ADDR + 1) = 0xAFB3001C;

}