#ifndef gamestateS_H
#define gamestateS_H

typedef enum gamestates {
    GAMEPLAY = 0,
    INTERACTING = 1,
    unk2 = 2,
    DYING = 3,
    PAUSED = 4,
    LOADING_CUTSCENE = 5,
    CUTSCENE = 6,
    LOADING_LEVEL = 7,
    unk8 = 8,
    unk9 = 9,
    CREDITS = 10,
    MAIN_MENU = 11,
    MENU_SPARX_UNK = 12,
    LOADING_SIDE_LEVEL = 13,
    FROZEN_UNK = 14,
    unkF = 15,
    FROZEN = 16,
    unk11 = 17,
    TRIGGER_CREDITS = 19
} gamestates;

#endif /* gamestateS_H */
