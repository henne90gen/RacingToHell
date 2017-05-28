#include <math.h>
#include <sstream>
#include <string.h>
#include "RacingToHell.h"
#include "platform.h"
#include "Font.h"

char *reservePermanentMemory(GameMemory *memory, size_t size)
{
    char *Result = memory->permanent + memory->permanentMemoryOffset;

    memory->permanentMemoryOffset += size;

    return Result;
}

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
	} else if (texture->y >= (int) buffer->height) {
		return;
	}

	unsigned startX = 0;
	if (texture->x < 0) {
		startX = 0 - texture->x;
	} else if (texture->x >= (int) buffer->width) {
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

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)
#define ABS(a) ((a < 0) ? -a : a)

void renderBackgroundTexture(VideoBuffer *buffer, Texture* texture) {
	if (texture->y < -(int32_t) texture->height
			|| texture->y > (int32_t) texture->height) {
		return;
	}

	uint32_t *currentTexturePixel = (uint32_t *) texture->content
			- MIN(texture->y * (int32_t )texture->width, 0);
	uint32_t *currentBufferPixel = (uint32_t *) buffer->content
			+ MAX(0, texture->y * (int32_t )buffer->width);

	uint32_t nextLine = buffer->width - texture->width;
	unsigned yMax = 0;

	if (texture->y > 0) {
		yMax = buffer->height - texture->y;
	} else if (texture->y < 0) {
		yMax = texture->height - ABS(texture->y);
	}

	for (unsigned y = 0; y < yMax; ++y) {
		for (unsigned x = 0; x < texture->width; ++x) {
			*currentBufferPixel++ = *currentTexturePixel++;
		}
		currentBufferPixel += nextLine;
	}
}

void renderTextureAlpha(VideoBuffer *buffer, Texture* texture, int offsetX,
		int offsetY) {
	int32_t *currentBufferPixel = (int32_t *) buffer->content
			+ offsetY * (int) buffer->width + offsetX;
	int32_t *currentTexturePixel = (int32_t *) texture->content;

	int32_t nextLine = buffer->width - texture->width;

	for (int y = 0; y < texture->height; ++y) {
		if (offsetY + y < 0) {
			currentBufferPixel += buffer->width;
			currentTexturePixel += texture->width;
			continue;
		} else if (offsetY + y >= buffer->height) {
			break;
		}

		for (int x = 0; x < texture->width; ++x) {
			if (offsetX + x < 0 || offsetX + x >= buffer->width) {
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

			if (textureA == 255) {
				*currentBufferPixel++ = *currentTexturePixel++;
			} else if (textureA == 0) {
				currentBufferPixel++;
				currentTexturePixel++;
			} else {
				uint8_t bufferA = *currentBufferPixel8;

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
		}

		currentBufferPixel += nextLine;
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
		abort(file.name + " is not a bitmap file.");
	}
	int fileHeaderSize = 14;
	BitmapHeader header = *((BitmapHeader*) (file.content + fileHeaderSize));

	if (header.bitsPerPixel != 32) {
		abort("Image must have 32-bit of color depth.");
	}

	Texture texture = { };
	texture.x = 0;
	texture.y = 0;
	texture.width = header.width;
	texture.height = header.height;
	texture.bytesPerPixel = header.bitsPerPixel / 8;
	texture.content = reservePermanentMemory(memory, texture.width * texture.height * texture.bytesPerPixel);

	importPixelData(file.content + header.size + fileHeaderSize,
			texture.content, texture.width, texture.height);
	printf("Successfully loaded %s.\n", file.name.c_str());
	return texture;
}

void loadTextures(GameMemory *memory, GameState *gameState) {
	for (int i = 0; i < 4; i++) {
		std::string filename = "./res/textures/roads/road" + std::to_string(i) + ".bmp";
		File file = readFile(filename);
		gameState->resources.roadTextures[i] = readBmpIntoMemory(file, memory);
        gameState->resources.roadTextures[i].y = -800;
		freeFile(&file);
	}
	for (int i = 0; i < 4; i++) {
		//printf("Texture %d: %d\n", i, roads[i].width * roads[i].height);
	}

	std::string carSprites = "./res/textures/cars/playercar2.bmp";
	File carFile = readFile(carSprites);
	gameState->resources.car = readBmpIntoMemory(carFile, memory);
	freeFile(&carFile);
}

void init(GameMemory *memory) {
    memory->isInitialized = true;

    GameState *gameState = (GameState *)reservePermanentMemory(memory, sizeof(GameState));

	*gameState = {};
	gameState->player = {};

	gameState->difficulty = 0;
	gameState->roadPosition = 0;
 
    font::loadFont(memory, "./res/font/arial.ttf");

    loadTextures(memory, gameState);

}

int getRoadSpeed(GameState *gameState) {
	// FIXME balance road speed
	return gameState->level * 1 + 3;
}

Texture* getCurrentRoad(GameState *gameState) {
	return &gameState->resources.roadTextures[gameState->level % 4];
}

void updateAndRenderRoad(VideoBuffer *buffer, GameState *gameState) {
	gameState->roadPosition += getRoadSpeed(gameState);
	if (gameState->roadPosition >= 800) {
		gameState->roadPosition = 0;
	}
	Texture* road = getCurrentRoad(gameState);
	road->y = gameState->roadPosition;
	renderBackgroundTexture(buffer, getCurrentRoad(gameState));
	road->y -= 799;
	renderBackgroundTexture(buffer, getCurrentRoad(gameState));
}


void renderDebugInformation(VideoBuffer *buffer, Input *input, GameState *gameState) {
	std::string text = "Player 1: " + std::to_string(gameState->player.x) + ", "
			+ std::to_string(gameState->player.y);
	font::renderText(buffer, "Abcdef", 100, 20, 14);
}

void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory) {
	if (!memory->isInitialized) {
		init(memory);
	}
//	printf("%d\n", counter++);
//	printf("RoadPosition: %d\n", gameState.roadPosition);

    GameState *gameState = (GameState *)(memory->permanent);

	updateAndRenderRoad(buffer, gameState);

	renderDebugInformation(buffer, input, gameState);
    /*
    renderTextureAlpha(buffer, &cars, -20, 780);
    renderTextureAlpha(buffer, &cars, -20, -20);
    renderTextureAlpha(buffer, &cars, 580, 780);
    renderTextureAlpha(buffer, &cars, 580, -20);
    renderTextureAlpha(buffer, &cars, 0, 400);
    renderTextureAlpha(buffer, &cars, 0, 500);

    renderTextureAlpha(buffer, &cars, 100, 0);
    renderTextureAlpha(buffer, &cars, 100, 100);
    renderTextureAlpha(buffer, &cars, 100, 200);
    renderTextureAlpha(buffer, &cars, 100, 300);
    renderTextureAlpha(buffer, &cars, 100, 400);
    renderTextureAlpha(buffer, &cars, 100, 500);

    renderTextureAlpha(buffer, &cars, 200, 0);
    renderTextureAlpha(buffer, &cars, 200, 100);
    renderTextureAlpha(buffer, &cars, 200, 200);
    renderTextureAlpha(buffer, &cars, 200, 300);
    renderTextureAlpha(buffer, &cars, 200, 400);
    renderTextureAlpha(buffer, &cars, 200, 500);

    renderTextureAlpha(buffer, &cars, 300, 0);
    renderTextureAlpha(buffer, &cars, 300, 100);
    renderTextureAlpha(buffer, &cars, 300, 200);
    renderTextureAlpha(buffer, &cars, 300, 300);
    renderTextureAlpha(buffer, &cars, 300, 400);
    renderTextureAlpha(buffer, &cars, 300, 500); */
}

void abort(std::string message) {
	fprintf(stderr, message.c_str());
	exit(1);
}
