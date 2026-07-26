#include <types.h>
#include <symbols.h>
#include <level_ids.h>
#include <gamestates.h>

bool HasBossFinished()
{
    if (gamestate == GAMEPLAY)
    {
        // Buzz
        if (currentLevel == BUZZS_DUNGEON)
        {
            return buzz_health == 0 && fade_effect == 0xFF;
        }
        // Spike
        if (currentLevel == SPIKES_ARENA)
        {
            return spike_health == 0 && fade_effect == 0xFF;
        }
        // Scorch
        if (currentLevel == SCORCHS_PIT)
        {
            return scorch_health == 0 && fade_effect == 0xFF;
        }
        // Sorc
        if (currentLevel == SORCERESS_LAIR)
        {
            return sorc_health == 0 && fade_effect == 0xFF;
        }
        // Sorc SBR
        const u32 final_sorc_sublevel_id = 3;
        if (currentLevel == SUPER_BONUS_ROUND)
        {
            return subLevelID == final_sorc_sublevel_id &&
                   final_sorc_health == 0 &&
                   fade_effect == 0xFF;
        }
    }
    return false;
}
