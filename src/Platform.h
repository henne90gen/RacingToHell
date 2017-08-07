#pragma once
#include <stdint.h>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <GL/glew.h>
#include <GL/wglew.h> 
#include <gl/GL.h>
#include <gl\GLU.h>
#include <amp.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glxext.h>
#endif

#define DEFAULT_WINDOW_WIDTH 1200
#define DEFAULT_WINDOW_HEIGHT 675
#define WINDOW_TITLE "Racing to Hell"
#define NUM_PLAYER_TEXTURES 6
#define NUM_TRAFFIC_TEXTURES 7
#define NUM_ITEM_TEXTURES 2
#define NUM_DIFFICULTIES 4
#define TOOLBOX_ID 0
#define CANISTER_ID 1

// Debug flags:
#define COLLISION_DEBUG 0
#define SOUND_DEBUG 0
#define SOUND_ENABLE 0

struct File {
	char* content;
	const void* fileHandle;
	size_t size;
	std::string name;
};

#define ABORT(name) void name(std::string message)
typedef ABORT(abort_);
ABORT(abort);

#define READ_FILE(name) File name(std::string fileName)
typedef READ_FILE(read_file);
READ_FILE(readFile);

#define WRITE_FILE(name) bool name(File *file, std::string fileName)
typedef WRITE_FILE(write_file);
WRITE_FILE(writeFile);

#define FREE_FILE(name) void name(File *file)
typedef FREE_FILE(free_file);
FREE_FILE(freeFile);

#define EXIT_GAME(name) void name()
typedef EXIT_GAME(exit_game);
EXIT_GAME(exitGame);

struct GameMemory {
	bool isInitialized = false;

	float aspectRatio;
	bool doResize;
	bool stretch = false;

	time_t shaderModTimes[2][2];
	char shaderFileNames[2][100];

	abort_ *abort;
	read_file *readFile;
	free_file *freeFile;
	exit_game *exitGame;

	char *temporary;
	size_t temporaryMemorySize = 0;
	size_t temporaryMemoryOffset = 0;

	char *permanent;
	size_t permanentMemorySize = 0;
	size_t permanentMemoryOffset = 0;
};
