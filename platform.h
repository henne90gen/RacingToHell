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

struct GameMemory {
	size_t temporaryMemorySize, permanentMemorySize;
	char *temporary, *permanent;

	size_t permanentMemoryOffset = 0;
	size_t temporaryMemoryOffset = 0;
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

File readFile(std::string fileName);
void freeFile(File *file);
