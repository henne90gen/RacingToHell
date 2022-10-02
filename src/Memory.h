#pragma once

#include "Platform.h"

char *reservePermanentMemory(Platform &platform, size_t size);
char *reserveTemporaryMemory(Platform &platform, size_t size);
void freeTemporaryMemory(Platform &platform);
