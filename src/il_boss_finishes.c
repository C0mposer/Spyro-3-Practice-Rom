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
            if (buzz_health == 0)
            {
                if (fade_effect == 0xFF)
                {
                    return true;
                }
            }
        }
        // Spike
        if (currentLevel == SPIKES_ARENA)
        {
            if (spike_health == 0)
            {
                if (fade_effect == 0xFF)
                {
                    return true;
                }
            }
        }
        // Scorch
        if (currentLevel == SCORCHS_PIT)
        {
            if (scorch_health == 0)
            {
                if (fade_effect == 0xFF)
                {
                    return true;
                }
            }
        }
        // Sorc
        if (currentLevel == SORCERESS_LAIR)
        {
            if (sorc_health == 0)
            {
                if (fade_effect == 0xFF)
                {
                    return true;
                }
            }
        }
        // Sorc SBR
        const u32 final_sorc_sublevel_id = 3;
        if (currentLevel == SUPER_BONUS_ROUND && subLevelID == final_sorc_sublevel_id)
        {
            if (final_sorc_health == 0)
            {
                if (fade_effect == 0xFF)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
