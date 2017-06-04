#include "RacingToHell.h"

char *reservePermanentMemory(GameMemory *memory, size_t size);
char *reserverTemporaryMemory(GameMemory *memory, size_t size);
void freeTemporaryMemory(GameMemory *memory);