#pragma once
#include <stdint.h>
#include <string>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "Racing to Hell"
#define NUM_PLAYER_TEXTURES 6
#define NUM_TRAFFIC_TEXTURES 7
#define NUM_ITEM_TEXTURES 2
#define TOOLBOX_ID 0
#define CANISTER_ID 1

// Debug flags:
#define COLLISION_DEBUG 1
#define SOUND_DEBUG 0
#define SOUND_ENABLE 0

struct File {
	char* content;
	const void* fileHandle;
	size_t size;
	std::string name;
};

struct GameMemory {
	bool isInitialized = false;

	size_t temporaryMemorySize = 0, permanentMemorySize = 0;
	char *temporary, *permanent;

	size_t permanentMemoryOffset = 0;
	size_t temporaryMemoryOffset = 0;
};

void abort(std::string message);
File readFile(std::string fileName);
void freeFile(File *file);
