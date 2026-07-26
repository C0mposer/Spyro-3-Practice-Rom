#ifndef SAVE_STATE_H
#define SAVE_STATE_H

#include <types.h>

extern u32 saveStateStoredBytes;

bool SaveStateCapture(void);
bool SaveStateRestore(void);

void FullSaveState(void);
void FullLoadState(void);

#endif // SAVE_STATE_H
