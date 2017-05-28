#pragma once
#include <iostream>
#include "platform.h"

struct VideoBuffer {

	uint32_t width, height, bytesPerPixel;
	void* content;
};

struct Texture {

	uint32_t width, height, bytesPerPixel;
	int x, y;
	void* content;
};

struct GameFont {
	size_t size;
	void* content;
};

struct Input {

	// TODO think about coordinate system for screen
	unsigned mouseX, mouseY;
	bool upKey, downKey, leftKey, rightKey;
	bool pauseKey;
	bool shootKey;
};

struct Player {
	float x, y;
	uint8_t health;
};

struct Resources
{
    Texture roadTextures[4];

    Texture car;
};

struct GameState {
	Player player;
	uint8_t level;
	uint8_t difficulty;
	uint32_t roadPosition;

    Resources resources;
};

#pragma pack(push)
struct BitmapHeader {
    uint32_t size; /* Size of this header in bytes */
    uint32_t width; /* Image width in pixels */
    uint32_t height; /* Image height in pixels */
    uint16_t planes; /* Number of color planes */
    uint16_t bitsPerPixel; /* Number of bits per pixel */
    uint32_t compression; /* Compression methods used */
    uint32_t sizeOfBitmap; /* Size of bitmap in bytes */
    uint32_t horzResolution; /* Horizontal resolution in pixels per meter */
    uint32_t vertResolution; /* Vertical resolution in pixels per meter */
    uint32_t colorsUsed; /* Number of colors in the image */
    uint32_t colorsImportant; /* Minimum number of important colors */
};
#pragma pack(pop)

Texture readBmpIntoMemory(File file, GameMemory *memory);

void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory);

void abort(std::string message);
