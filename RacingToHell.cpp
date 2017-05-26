#include "RacingToHell.h"
#include "platform.h"
#include <math.h>
#include <sstream>
#include <string.h>

static bool loaded = false;
static int counter = 0;

static Texture roads[4];
static Texture cars;
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
			((uint32_t*) buffer->content)[y * buffer->width + x] = color
					+ (255 << 24);
		}
	}
}

void renderTexture(VideoBuffer *buffer, Texture* texture) {
	unsigned startY = 0;
	if (texture->y < 0) {
		startY = 0 - texture->y;
		printf("setting new startY");
	} else if (texture->y >= (int)buffer->height) {
		return;
	}

	unsigned startX = 0;
	if (texture->x < 0) {
		startX = 0 - texture->x;
	} else if (texture->x >= (int)buffer->width) {
		return;
	}

	for (unsigned y = startY; y < texture->height; y++) {
		if (texture->y + y >= buffer->height) {
			continue;
		}
		for (unsigned x = startX; x < texture->width; x++) {
			if (texture->x + x >= buffer->width) {
				continue;
			}
			int bufferIndex = buffer->width * (texture->y + y) + texture->x + x;
			int textureIndex = y * texture->width + x;
			((int*) buffer->content)[bufferIndex] =
					((int*) (texture->content))[textureIndex];
		}
	}
}

void renderTextureAlpha(VideoBuffer *buffer, Texture* texture, int offsetX,
		int offsetY) {
	uint32_t *currentBufferPixel = (uint32_t *) buffer->content
			+ offsetY * buffer->width + offsetX;
	uint32_t *currentTexturePixel = (uint32_t *) texture->content;

	for (unsigned y = 0; y < texture->height; ++y) {
		for (unsigned x = 0; x < texture->width; ++x) {
			if (offsetX + x >= buffer->width || offsetY + y >= buffer->height) {
				currentBufferPixel++;
				currentTexturePixel++;
				continue;
			}

			uint8_t *currentBufferPixel8 = (uint8_t *) currentBufferPixel;
			uint8_t *currentTexturePixel8 = (uint8_t *) currentTexturePixel;

			uint8_t textureB = *currentTexturePixel8++;
			uint8_t textureG = *currentTexturePixel8++;
			uint8_t textureR = *currentTexturePixel8++;
			uint8_t textureA = *currentTexturePixel8++;

			uint8_t bufferA = *(uint8_t *) currentBufferPixel;

			float textureAlpha = textureA / 255.0f;
			float bufferAlpha = bufferA / 255.0f;

			float resultAlpha = textureAlpha
					+ bufferAlpha * (1.0f - textureAlpha);

			if (resultAlpha == 0.0f) {
				*currentBufferPixel = 0;
			} else {
				float newB = ((bufferAlpha * (1.0f - textureAlpha)
						* *currentBufferPixel8) + (textureAlpha * textureB))
						/ resultAlpha;
				*currentBufferPixel8++ = (uint8_t) newB;

				float newG = ((bufferAlpha * (1.0f - textureAlpha)
						* *currentBufferPixel8) + (textureAlpha * textureG))
						/ resultAlpha;
				*currentBufferPixel8++ = (uint8_t) newG;

				float newR = ((bufferAlpha * (1.0f - textureAlpha)
						* *currentBufferPixel8) + (textureAlpha * textureR))
						/ resultAlpha;
				*currentBufferPixel8++ = (uint8_t) newR;

				float newA = resultAlpha * 255.0f;
				*currentBufferPixel8++ = (uint8_t) newA;
			}

			currentBufferPixel++;
			currentTexturePixel++;
		}

		currentBufferPixel = (uint32_t *) buffer->content
				+ (offsetY + y + 1) * buffer->width + offsetX;
	}
}

void importPixelData(void* input, void* output, unsigned width,
		unsigned height) {
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			int inputIndex = (height - y - 1) * width + x;
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
	for (int i = 0; i < 4; i++) {
		//printf("Texture %d: %d\n", i, roads[i].width * roads[i].height);
	}

	std::string carSprites = "./res/textures/cars/cars.bmp";
	File carFile = readFile(carSprites);
	cars = readBmpIntoMemory(carFile, memory);
	freeFile(&carFile);
}

void loadFont(GameMemory *memory) {
	File file = readFile("./res/font/arial.ttf");
	GameFont font = { };
	font.size = file.size;
	font.content = file.content;
	printf("Font size: %d\n", (int) font.size);
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
//	renderTextureAlpha(buffer, getCurrentRoad(), getCurrentRoad()->x,
//			getCurrentRoad()->y);
	renderTexture(buffer, getCurrentRoad());
	road->y -= 799;
//	renderTextureAlpha(buffer, getCurrentRoad(), getCurrentRoad()->x,
//			getCurrentRoad()->y);
	renderTexture(buffer, getCurrentRoad());
}

void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory) {
	if (!loaded) {
		init(memory);
	}
//	printf("%d\n", counter++);

//	clearScreen(buffer, 0);
	//printf("RoadPosition: %d\n", gameState.roadPosition);

//	updateAndRenderRoad(buffer);

//	renderTextureAlpha(buffer, &cars, 0, 0);

	//clearScreen(buffer, ((int)(input->upKey) * 255) + (((int)(input->downKey) * 255) << 8) + (((int)(input->shootKey) * 255) << 16));
}
