#include "menu/menu.h"
#include <gamestates.h>
#include <level_ids.h>
#include <symbols.h>
#include <types.h>

#define SORC_CANNON_SLOTS_ADDRESS 0x801607F0
#define SORC_CANNON_SLOT_COUNT 5
#define SORC_CANNON_LAYOUT_COUNT 10
#define SORC_LAYOUT_FINISHED 0x80

typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    s32 occupiedBy;
} SorcCannonSlot;

static const u8 sorcCannonLayoutMasks[SORC_CANNON_LAYOUT_COUNT] =
{
    0x1A, // ABC
    0x19, // ABD
    0x1C, // ABE
    0x0B, // ACD
    0x0E, // ACE
    0x0D, // ADE
    0x13, // BCD
    0x16, // BCE
    0x15, // BDE
    0x07  // CDE
};

// 0 means waiting, 1-10 means active, and bit 7 means finished
static u8 layoutState;

static void SetUnwantedSlots(SorcCannonSlot* slots, u8 menuOption, s32 occupiedBy)
{
    u8 allowedSlotMask = sorcCannonLayoutMasks[menuOption - 1];
    int i;

    for (i = 0; i < SORC_CANNON_SLOT_COUNT;
         i++, allowedSlotMask >>= 1)
    {
        if ((allowedSlotMask & 1) == 0)
        {
            slots[i].occupiedBy = occupiedBy;
        }
    }
}

extern u32 g_ILTimerMode;
void SorcLayoutForceUpdate(void)
{
    SorcCannonSlot* slots = (SorcCannonSlot*)SORC_CANNON_SLOTS_ADDRESS;
    u8 menuOption = main_menu.elements[SORC_LAYOUT_TRAINER_TOGGLE].selection_option;
    u8 activeMenuOption;
    u8 allowedSlotMask;
    int selectedCount;
    int i;

    // Don't allow changing sorc cannons when IL mode is on
    if (g_ILTimerMode > 0) // Is > 0, aka on
    {
        main_menu.elements[SORC_LAYOUT_TRAINER_TOGGLE].selection_option = 0;
    }

    if (currentLevel != SORCERESS_LAIR ||
        gamestate == DYING ||
        gamestate == LOADING_LEVEL)
    {
        layoutState = 0;
        return;
    }

    // Wait until the cannon table has inited
    if (slots[0].x != 56617)
    {
        return;
    }

    activeMenuOption = layoutState & ~SORC_LAYOUT_FINISHED;

    // Check if menu option changed
    if (activeMenuOption != 0 && activeMenuOption != menuOption)
    {
        if ((layoutState & SORC_LAYOUT_FINISHED) == 0)
        {
            SetUnwantedSlots(slots, activeMenuOption, 0);
        }

        layoutState = 0;
    }

    if (menuOption == 0 ||
        (layoutState & SORC_LAYOUT_FINISHED) != 0)
    {
        return;
    }

    allowedSlotMask = sorcCannonLayoutMasks[menuOption - 1];

    if (layoutState == 0)
    {
        // Do not change after Agent 9 has chosen a canon
        for (i = 0; i < SORC_CANNON_SLOT_COUNT; i++)
        {
            if (slots[i].occupiedBy != 0)
            {
                return;
            }
        }

        // Block the two unwanted positions
        SetUnwantedSlots(slots, menuOption, 1);
        layoutState = menuOption;
        return;
    }

    selectedCount = 0;

    for (i = 0; i < SORC_CANNON_SLOT_COUNT;
         i++, allowedSlotMask >>= 1)
    {
        if ((allowedSlotMask & 1) &&
            slots[i].occupiedBy != 0)
        {
            selectedCount++;
        }
    }

    if (selectedCount == 3)
    {
        SetUnwantedSlots(slots, menuOption, 0);
        layoutState = menuOption | SORC_LAYOUT_FINISHED;
    }
}
