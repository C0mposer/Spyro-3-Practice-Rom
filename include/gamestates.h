#ifndef gamestateS_H
#define gamestateS_H

typedef enum gamestates {
    gameplay = 0,
    interacting = 1,
    unk2 = 2,
    dying = 3,
    paused = 4,
    loadingCutscene = 5,
    cutscene = 6,
    loadingLevel = 7,
    unk8 = 8,
    unk9 = 9,
    credits = 10,
    mainMenu = 11,
    menusparxUnk = 12,
    loadingSideLevel = 13,
    frozen = 14,
    unkF = 15,
    fireworksMatrixCutscene = 16,
    unk11 = 17,
    triggerCredits = 19
} gamestates;

#endif /* gamestateS_H */
