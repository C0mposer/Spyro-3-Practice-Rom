#include <types.h>
#include <symbols.h>
#include <bianca_flags.h>

#define SUNRISE_SPRING_ID  0x0A
#define SUNNY_VILLA_ID     0x0B
#define SUNNY_LEVEL_INDEX  1

// For practicing the Sunny pause buffer SiA
void SunnyExit(void)
{
    if (currentLevel != SUNRISE_SPRING_ID)
    {
        return;
    }

    // Set up vars to exit sunny villa portal
    currentLevel = SUNNY_VILLA_ID;
    levelIndex = SUNNY_LEVEL_INDEX;
    isHomeworldLevel = 0;

    // This is how the game normally loads the homeworld of the current level. -1 means return to the current level's homeworld.
    BeginLevelLoad(-1);

    progressFlags[BIANCA_SUNRISE_FLAG_OFFSET] = true; // Set bianca in sunrise to be talked to, so we don't talk to her when exiting
}

// For practicing the Sheila pause buffer SiA
void SheilaExit()
{
    savedCheckpointUpdated = 1;

    // The data loaded from the game, when respawning out of Sheila after the cutscene
    respawnPosition.x = 0x8F70;
    respawnPosition.y = 0xF250;
    respawnPosition.z = 0x5368;
    respawnAngle = 0xBF0;
    checkpointCameraMode = 1;
    savedCheckpointSwimState = 0;

    previousLevelIDForVehicleEntry = 0;

    RespawnSpyro();
}
