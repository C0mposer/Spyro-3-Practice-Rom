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
static bool hasSavedSpyroPosition = false;

static Vec3 savedSpyroPosition = { 0 };
static s32 savedSpyroYawAngle = 0;
static u32 savedPositionLevelID = 0;
static u32 savedPositionSubLevelID = 0;



inline static void SpeedUpReset(void)
{
    const u32 minimumLayoutWait = 6;
    const u32 normalDeathLayoutWait = 0x50;

    // Skip the artificial death delay. It still waits until the CD read completes.
    if (speedUpResetPending &&
        gamestate == DYING &&
        menuState == 1 &&
        framesInScenario >= minimumLayoutWait)
    {
        framesInScenario = normalDeathLayoutWait;
        speedUpResetPending = false;
    }
}

// Fast Load (toggle-gated): speeds up the death/respawn scenario that both resets
// ride. That scenario (gamestate DYING, driven by UpdateDuringFadeIn) is a 5-phase
// menuState machine: 0 fade-out, 1 CD wait, 2 layout load, 3 setup, 4 fade-in.
// Both modes cut the fade-out (phase 0) and the artificial wait (phase 1), and
// neither touches the real CD read or the phase-2 moby/layout reload. They differ
// ONLY on the fade-in, because that's where enemy cycles live:
//
//   FAST_LOAD_CUT_BOTH_FADES  -> saved-position teleport (L1+R1+Square, and the
//       disabled-portal touch respawn). Also cuts the fade-in for the fastest
//       possible reset. Cutting the fade-in drops ~16 enemy-update frames (mobys
//       only tick in phases 3-4), so enemy cycles shift -- irrelevant when you're
//       teleporting to an arbitrary saved spot, not practicing cycles.
//
//   FAST_LOAD_KEEP_FADE_IN    -> level-start reset (L1+R1+Triangle). Cuts the
//       fade-OUT only. Phases 0-1 tick no mobys, so removing the fade-out is
//       cycle-neutral; keeping the fade-in preserves the ~16 enemy-update frames a
//       legit reset has, so enemy cycles stay deterministic for practicing a run
//       from level start.
typedef enum FastLoadFadeMode
{
    FAST_LOAD_CUT_BOTH_FADES,
    FAST_LOAD_KEEP_FADE_IN
} FastLoadFadeMode;

bool fastLoadActive = false;
bool fastLoadInScenario = false;
static FastLoadFadeMode fastLoadFadeMode = FAST_LOAD_CUT_BOTH_FADES;

bool FastLoadEnabled(void)
{
    printf_syscall("FAST_LOAD_TOGGLE = %d, Enabled = %d\n", FAST_LOAD_TOGGLE, main_menu.elements[FAST_LOAD_TOGGLE].enabled);
    return main_menu.elements[FAST_LOAD_TOGGLE].enabled != 0;
}

inline static void FastLoadUpdate(void)
{
    const u32 fadeFullBlack = 0xFF;    // phase 0: jump straight to black
    const u32 fadeInFinishStep = 0x10; // phase 4: ROM subtracts 0x10 then completes at < 1
    const u32 skipArtificialWait = 0x50;

    if (!fastLoadActive)
    {
        return;
    }

    if (gamestate == DYING)
    {
        fastLoadInScenario = true;

        switch (menuState)
        {
            case 0: // fade-out: force full black so the ROM advances immediately
            drawScreenBlack = fadeFullBlack;
            break;
            case 1: // CD wait: skip the artificial delay (WaitForCd still gates the real read)
            if (framesInScenario != 0 && framesInScenario < skipArtificialWait)
            {
                framesInScenario = skipArtificialWait;
            }
            break;
            case 4: // fade-in: cut ONLY for the saved-position teleport. The level-start
                    //          reset keeps it so enemy cycles match a real reset (mobys
                    //          tick during the fade-in).
            if (fastLoadFadeMode == FAST_LOAD_CUT_BOTH_FADES &&
                drawScreenBlack > fadeInFinishStep)
            {
                drawScreenBlack = fadeInFinishStep;
            }
            break;
            default:
            break;
        }
    }
    else if (fastLoadInScenario && gamestate == GAMEPLAY)
    {
        // Scenario returned to gameplay; disarm and default the mode back for next time.
        fastLoadActive = false;
        fastLoadInScenario = false;
        fastLoadFadeMode = FAST_LOAD_CUT_BOTH_FADES;
    }
}

inline static void SaveStartingPositionUpdate(void)
{
    static u32 currentLevelID = 0;
    static u32 currentSubLevelID = 0;

    if (gamestate == GAMEPLAY)
    {
        if (currentLevelID != levelID)
        {

            Vec3Copy(&savedStartingPosition, &respawnPosition); // Save the respawn position for the current level/sub level
            savedStartingAngle = respawnAngle; // Save the respawn position for the current level/sub level

            currentLevelID = levelID;

        }
        if (currentSubLevelID != subLevelID)
        {
            Vec3Copy(&savedStartingPositionSubLevel, &respawnPosition); // Save the respawn position for the current level/sub level
            savedStartingAngleSubLevel = respawnAngle; // Save the respawn position for the current level/sub level

            currentSubLevelID = subLevelID;

        }
    }
}

inline static void ReloadLevelStartingPosition(void)
{
    if (subLevelID == 0) {
        Vec3Copy(&respawnPosition, &savedStartingPosition);
        respawnAngle = savedStartingAngle;
    }
    else
    {
        Vec3Copy(&respawnPosition, &savedStartingPositionSubLevel);
        respawnAngle = savedStartingAngleSubLevel;
    }
}

inline static bool HasSavedSpyroPositionForCurrentLevel(void)
{
    return hasSavedSpyroPosition &&
        savedPositionLevelID == levelID &&
        savedPositionSubLevelID == subLevelID;
}

// Manually save spyro's position
inline static void ManualSaveSpyroPositionUpdate(void)
{
    const u32 saveCombo = SAVE_SPYRO_HOTKEY;
    const u32 savedMessageDuration = 60;
    static bool saveComboWasHeld = false;
    static bool showSavedMessage = false;
    static u32 savedMessageStartTime = 0;
    bool saveComboIsHeld = (isButtonHeld == saveCombo);

    if (saveComboIsHeld && !saveComboWasHeld)
    {
        savedSpyroPosition.x = spyroX;
        savedSpyroPosition.y = spyroY;
        savedSpyroPosition.z = spyroZ;
        savedSpyroYawAngle = spyroYawAngle;

        savedPositionLevelID = levelID;
        savedPositionSubLevelID = subLevelID;
        hasSavedSpyroPosition = true;
        savedMessageStartTime = levelTimer;
        showSavedMessage = true;
    }

    saveComboWasHeld = saveComboIsHeld;

    if (showSavedMessage)
    {
        if ((u32)(levelTimer - savedMessageStartTime) < savedMessageDuration)
        {
            DrawText("Saved", 10, 215, 0x1, 0);
        }
        else
        {
            showSavedMessage = false;
        }
    }
}

void PrepareSavedSpyroRespawn(void)
{
    if (HasSavedSpyroPositionForCurrentLevel())
    {
        // Set spyro's respawn position, to the saved position
        savedCheckpointUpdated = 1;
        Vec3Copy(&respawnPosition, &savedSpyroPosition);
        respawnAngle = (u32)savedSpyroYawAngle;
    }
    else
    {
        ReloadLevelStartingPosition();
    }
}

void RespawnSpyro(void)
{
    spyroZ = 0; // Kill spyro by sending him to the void
}

void ClearCollectables(void)
{
    // Clear GUI counters.
    globalGems = 0;
    globalEggs = 0;

    // Clear gem flags
    memset(gemsCollectedPerLevel, 0, 0xA0);
    memset(gemsCollectedFlags, 0, 0x500);

    // Clear egg collection flags and egg difficulty history
    memset(eggsCollectedBitmask, 0, 0x28);
    eggsCollectedSinceEnteredBitmask = 0;
    memset(eggDifficultyState, 0, 0xF0);
    memset(recordedPerformancePerEgg, 0, 0xF0);

    // Clear progress flags
    memset(progressFlags, 0, 0x4C);
    memset(dialogueFlags, 0, 0x28);
    memset(skillPointFlags, 0, 0x14);

    // Reset boss death counters (they can override adaptive difficulty sometimes)
    deathsToBuzz = 0;
    deathsToSpike = 0;
    deathsToScorch = 0;
    deathsToSorceress = 0;

    totalHits = 0;
    MemCopy(sparxState, sparxDefaults, 0x28);

    // Reset checkpoint data
    memset(currentCheckpointData, 0, 0x850);
    memset(savedCheckpointData, 0, 0x850);
}

inline static void UnlockAtlasWarp()
{
    memset(hasEnteredLevelFlags, 0x11111111, 40); // Unlock all levels in atlas

    // Check if the atlas overlay is loaded. If so, nop the sparx breaking basket checks, to force atlas warp to work
    opcode_as_u32* check_1 = ((opcode_as_u32*)0x800791DC);
    opcode_as_u32* check_2 = ((opcode_as_u32*)0x8007755C);
    bool is_check_1_loaded = *check_1 == 0x14400021;
    bool is_check_2_loaded = *check_2 == 0x14400011;

    printf_syscall("check1: %X, check2: %X\n", *check_1, *check_2);
    if (is_check_1_loaded && is_check_2_loaded)
    {
        *check_1 = 0x0;
        *check_2 = 0x0;
    }
}

// Basic checks that should run every frame
void MainUpdates(void)
{
    SpeedUpReset();
    FastLoadUpdate();

    if (gamestate == PAUSED)
    {
        UnlockAtlasWarp();
    }

    if (gamestate == GAMEPLAY || gamestate == INTERACTING)
    {
        //lives = 4;
        *((u32*)0x80049a50) = 0x0; // NOP the life decrement code during a death. Less annoying than having lives set to 4 over and over, with sfx. 

        SaveStartingPositionUpdate();
        ManualSaveSpyroPositionUpdate();

        // Restart the level from the saved position
        if (isButtonHeld == LOAD_SPYRO_HOTKEY)
        {
            ClearCollectables();
            PrepareSavedSpyroRespawn();
            speedUpResetPending = true;
            if (FastLoadEnabled())
            {
                fastLoadActive = true;
                fastLoadInScenario = false;
                fastLoadFadeMode = FAST_LOAD_CUT_BOTH_FADES;
            }
            RespawnSpyro();
        }
        // Restart the level from the spawn point
        else if (isButtonHeld == RELOAD_LEVEL_HOTKEY)
        {
            ClearCollectables();
            ReloadLevelStartingPosition();
            speedUpResetPending = true;
            if (FastLoadEnabled())
            {
                fastLoadActive = true;
                fastLoadInScenario = false;
                fastLoadFadeMode = FAST_LOAD_KEEP_FADE_IN;
            }
            RespawnSpyro();
        }
    }
}
