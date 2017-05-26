#include "RacingToHell.h"
#include "platform.h"
#include <math.h>
#include <sstream>
#include <string.h>

static bool loaded = false;

static Texture roads[4];
static GameState gameState;

void testInput(Input* input) {
	printf("(%d|%d)\n", input->mouseX, input->mouseY);
	if (input->upKey) {
		printf("Going up!\n");
	}
	if (input->downKey) {
		printf("Going down!\n");
	}
	if (input->leftKey) {
		printf("Going left!\n");
	}
	if (input->rightKey) {
		printf("Going right!\n");
	}
	if (input->shootKey) {
		printf("Shooting!\n");
	}
}

void clearScreen(VideoBuffer *buffer, int color) {
	for (unsigned int y = 0; y < buffer->height; y++) {
		for (unsigned int x = 0; x < buffer->width; x++) {
			((int*) buffer->content)[y * buffer->width + x] = color;
		}
	}
}

void renderTexture(VideoBuffer *buffer, Texture* texture) {
	for (unsigned y = 0; y < texture->height; y++) {
		for (unsigned x = 0; x < texture->width; x++) {
			if (texture->x + x >= buffer->width
					|| texture->y + y >= buffer->height) {
				continue;
			}
			int bufferIndex = buffer->width * (texture->y + y) + texture->x + x;
			int textureIndex = y * texture->width + x;
			((int*) buffer->content)[bufferIndex] =
					((int*) texture->content)[textureIndex];
		}
	}
}

void importPixelData(void* input, void* output, unsigned width,
		unsigned height) {
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			int inputIndex = (height - y) * width + x;
			int outputIndex = y * width + x;
			uint32_t color = ((uint32_t*) (input))[inputIndex];
			uint8_t r = (color & 0xff000000) >> 24;
			uint8_t g = (color & 0x00ff0000) >> 16;
			uint8_t b = (color & 0x0000ff00) >> 8;
			uint8_t a = color & 0x000000ff;
			color = (a << 24) + (r << 16) + (g << 8) + b;
			((uint32_t*) (output))[outputIndex] = color;
		}
	}
}

Texture readBmpIntoMemory(File file, GameMemory *memory) {
	if (((char*) file.content)[0] != 'B' || (file.content)[1] != 'M') {
		fprintf(stderr, "%s is not a .bmp file.\n", file.name.c_str());
		exit(1);
	}
	int fileHeaderSize = 14;
	BitmapHeader header = *((BitmapHeader*) (file.content + fileHeaderSize));

	if (header.bitsPerPixel != 32) {
		fprintf(stderr, "Image must have 32-bit colors.");
		exit(1);
	}

	Texture texture = { };
	texture.x = 0;
	texture.y = 0;
	texture.width = header.width;
	texture.height = header.height;
	texture.bytesPerPixel = header.bitsPerPixel / 8;
	texture.content = memory->permanent + memory->permanentMemoryOffset;

	memory->permanentMemoryOffset += header.width * header.height
			* texture.bytesPerPixel;

	importPixelData(file.content + header.size + fileHeaderSize,
			texture.content, header.width, header.height);
	printf("Successfully loaded %s.\n", file.name.c_str());
	return texture;
}

void loadTextures(GameMemory *memory) {
	for (int i = 0; i < 4; i++) {
		std::stringstream fileName;
		fileName << "./res/textures/roads/road" << i << ".bmp";
		File file = readFile(fileName.str());
		roads[i] = readBmpIntoMemory(file, memory);
		roads[i].y = -800;
		freeFile(&file);
	}
}

void loadFont(GameMemory *memory) {
	File file = readFile("./res/font/arial.ttf");
	GameFont font = {};
	font.size = file.size;
	font.content = file.content;
	printf("Font size: %d\n", (int)font.size);
	return;
}

void init(GameMemory *memory) {
	loaded = true;
	loadFont(memory);
	loadTextures(memory);

	gameState = {};
	gameState.player = {};
	gameState.level = 0;
	gameState.difficulty = 0;
	gameState.roadPosition = 0;
}

int getRoadSpeed() {
	// FIXME balance road speed
	return gameState.level * 10 + 10;
}

Texture* getCurrentRoad() {
	return &roads[gameState.level % 4];
}

void updateAndRenderRoad(VideoBuffer *buffer) {
	gameState.roadPosition += getRoadSpeed();
	if (gameState.roadPosition >= 800) {
		gameState.roadPosition = 0;
	}
	Texture* road = getCurrentRoad();
	road->y = gameState.roadPosition;
	renderTexture(buffer, getCurrentRoad());
	road->y -= 799;
	renderTexture(buffer, getCurrentRoad());
}

void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory) {
	if (!loaded) {
		init(memory);
	}
	clearScreen(buffer, 0);

	updateAndRenderRoad(buffer);


//	clearScreen(buffer, ((int)(input->upKey) * 255) + (((int)(input->downKey) * 255) << 8) + (((int)(input->shootKey) * 255) << 16));
}
