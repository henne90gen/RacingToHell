#include "Platform.h"

char *reservePermanentMemory(Platform *platform, size_t size) {
    if (platform->memory.permanentMemoryOffset + size > platform->memory.permanentMemorySize) {
        platform->abort("Out of permanent memory!");
    }

    char *Result = platform->memory.permanent + platform->memory.permanentMemoryOffset;
    platform->memory.permanentMemoryOffset += size;
    return Result;
}

char *reserveTemporaryMemory(Platform *platform, size_t size) {
    if (platform->memory.temporaryMemoryOffset + size > platform->memory.temporaryMemorySize) {
        platform->abort("Out of temporary memory!");
    }

    char *Result = platform->memory.temporary + platform->memory.temporaryMemoryOffset;
    platform->memory.temporaryMemoryOffset += size;
    return Result;
}

void freeTemporaryMemory(Platform *platform) { platform->memory.temporaryMemoryOffset = 0; }
