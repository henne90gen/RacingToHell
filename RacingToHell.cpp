#include <math.h>
#include <sstream>
#include <string.h>
#include "RacingToHell.h"
#include "platform.h"
#include "Renderer.h"
#include "Font.h"

#include "Helper.cpp"

void loadTextures(GameMemory *memory, GameState *gameState) {
	for (int i = 0; i < 4; i++) {
		std::string filename = "./res/textures/roads/road" + std::to_string(i)
				+ ".bmp";
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

	GameState *gameState = (GameState *) reservePermanentMemory(memory,
			sizeof(GameState));

	*gameState = {};
	gameState->player = {};

	gameState->level = 0;
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
	render::backgroundTexture(buffer, getCurrentRoad(gameState));
	road->y -= 799;
	render::backgroundTexture(buffer, getCurrentRoad(gameState));
}

GameState* getGameState(GameMemory* memory) {
	if (!memory->isInitialized) {
		init(memory);
	}
	return (GameState *) (memory->permanent);
}

void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory) {
	GameState *gameState = getGameState(memory);

//	printf("%d\n", counter++);
//	printf("RoadPosition: %d\n", gameState.roadPosition);

	render::clearScreen(buffer, 0);

	updateAndRenderRoad(buffer, gameState);

	render::debugInformation(buffer, input, gameState);
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

