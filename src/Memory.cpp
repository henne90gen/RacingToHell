#include "Memory.h"

char *reservePermanentMemory(GameMemory *memory, size_t size) {
    if (memory->permanentMemoryOffset + size > memory->permanentMemorySize) {
        abort("Out of permanent memory!");
    }

    char *Result = memory->permanent + memory->permanentMemoryOffset;

    memory->permanentMemoryOffset += size;

    return Result;
}

char *reserveTemporaryMemory(GameMemory *memory, size_t size)
{
    if (memory->temporaryMemoryOffset + size > memory->temporaryMemorySize) {
        abort("Out of temporary memory!");
    }

    char *Result = memory->temporary + memory->temporaryMemoryOffset;

    memory->temporaryMemoryOffset += size;

    return Result;
}

void freeTemporaryMemory(GameMemory *memory)
{
    memory->temporaryMemoryOffset = 0;
}
