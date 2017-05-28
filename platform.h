#pragma once
#include <stdint.h>
#include <string>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "Racing to Hell"

struct File {
	char* content;
	size_t size;
	std::string name;
};

struct GameMemory
{
    bool isInitialized = false;

    size_t temporaryMemorySize, permanentMemorySize;
    char *temporary, *permanent;

    size_t permanentMemoryOffset = 0;
    size_t temporaryMemoryOffset = 0;
};

File readFile(std::string fileName);
void freeFile(File *file);
