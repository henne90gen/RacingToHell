#include <math.h>
#include <sstream>
#include <string.h>

#include "RacingToHell.h"
#include "platform.h"
#include "Renderer.h"
#include "Font.h"
#include "Math.h"

#include "Helper.cpp"

void loadTextures(GameMemory *memory, GameState *gameState) {
	for (int i = 0; i < 4; i++) {
		std::string filename = "./res/textures/roads/road" + std::to_string(i)
				+ ".bmp";
		File file = readFile(filename);
		gameState->resources.roadTextures[i] = readBmpIntoMemory(file, memory);
		freeFile(&file);
	}
	for (int i = 0; i < 4; i++) {
		//printf("Texture %d: %d\n", i, roads[i].width * roads[i].height);
	}

	for (unsigned i = 0; i < 6; i++) {
		std::string carSprites = "./res/textures/cars/player"
				+ std::to_string(i) + ".bmp";
		File carFile = readFile(carSprites);
		gameState->resources.playerCarTextures[i] = readBmpIntoMemory(carFile,
				memory);
		freeFile(&carFile);
	}

	for (unsigned i = 0; i < 7; i++) {
		std::string carSprites = "./res/textures/cars/traffic"
				+ std::to_string(i) + ".bmp";
		File carFile = readFile(carSprites);
		gameState->resources.trafficCarTextures[i] = readBmpIntoMemory(carFile,
				memory);
		freeFile(&carFile);
	}

	File explosionFile = readFile("./res/textures/explosion.bmp");
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			int offsetX = x * 100;
			int offsetY = y * 100;
			int width = 100;
			int height = 100;
			bool output = y == 8 && x == 8;
			gameState->resources.explosion[y * 9 + x] = readBmpIntoMemory(
					explosionFile, memory, offsetX, offsetY, width, height,
					output);
		}
	}
	freeFile(&explosionFile);
}

void init(GameMemory *memory) {
	memory->isInitialized = true;

	GameState *gameState = (GameState *) reservePermanentMemory(memory,
			sizeof(GameState));

	*gameState = {};
	gameState->player = {};
	gameState->player.position.x = (float) (WINDOW_WIDTH / 2);
	gameState->player.position.y = (float) (WINDOW_HEIGHT / 2);
	gameState->player.speed = 10;

	gameState->level = 0;
	gameState->difficulty = 0;
	gameState->roadPosition = 0;
	gameState->frameCounter = 0;

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
	render::backgroundTexture(buffer, getCurrentRoad(gameState),
			gameState->roadPosition);
	render::backgroundTexture(buffer, getCurrentRoad(gameState),
			gameState->roadPosition - 800);
}

GameState* getGameState(GameMemory* memory) {
	if (!memory->isInitialized) {
		init(memory);
	}

	return (GameState *) (memory->permanent);
}

void spawnBullet(GameState *gameState, Math::Vector2f position,
		Math::Vector2f velocity, bool playerBullet) {
	Bullet bullet = { };
	bullet.position = position;
	bullet.velocity = velocity;

	if (playerBullet) {
		int arrSize = sizeof(gameState->playerBullets) / sizeof(Bullet);
		if (gameState->lastPlayerIndex + 1 < arrSize) {
//			if (gameState->lastPlayerIndex < -1
//					|| gameState->lastPlayerIndex > 100) {
//				abort("Fail 3");
//			}
			gameState->lastPlayerIndex++;
			gameState->playerBullets[gameState->lastPlayerIndex] = bullet;
//			if (gameState->lastPlayerIndex < -1
//					|| gameState->lastPlayerIndex > 100) {
//				abort("Fail 3");
//			}
		}
	}
}

void updateAndRenderPlayer(VideoBuffer *buffer, Input *input,
		GameState *gameState) {
	int x = 0;
	int y = 0;
	if (input->downKey) {
		y += gameState->player.speed;
	}
	if (input->upKey) {
		y -= gameState->player.speed;
	}
	if (input->leftKey) {
		x -= gameState->player.speed;
	}
	if (input->rightKey) {
		x += gameState->player.speed;
	}

	if (x || y) {
		Math::Vector2f movement = { };
		movement.x = x;
		movement.y = y;
		movement = Math::normalize(movement);
		movement = movement * gameState->player.speed;
		gameState->player.position = gameState->player.position + movement;
	}

	if (input->shootKey) {
		Math::Vector2f velocity = input->mousePosition
				- gameState->player.position;
		velocity = velocity * (1.0 / Math::length(velocity));
		velocity = velocity * 5;
		spawnBullet(gameState, gameState->player.position, velocity, true);
	}

	Texture *texture =
			&gameState->resources.playerCarTextures[gameState->player.carIndex];
	// checking left and right
	if (gameState->player.position.x < texture->width / 2) {
		gameState->player.position.x = texture->width / 2;
	} else if (gameState->player.position.x
			> WINDOW_WIDTH - texture->width / 2) {
		gameState->player.position.x = WINDOW_WIDTH - texture->width / 2;
	}

	// checking top and bottom
	if (gameState->player.position.y < texture->height / 2) {
		gameState->player.position.y = texture->height / 2;
	} else if (gameState->player.position.y
			> WINDOW_HEIGHT - texture->height / 2) {
		gameState->player.position.y = WINDOW_HEIGHT - texture->height / 2;
	}

	int textureX = gameState->player.position.x - texture->width / 2;
	int textureY = gameState->player.position.y - texture->height / 2;
	render::textureAlpha(buffer, texture, textureX, textureY);
}

bool updateAndRenderBullet(VideoBuffer* buffer, Bullet &bullet) {
	bullet.position = bullet.position + bullet.velocity;

	if (bullet.position.x < 0) {
		return true;
	} else if (bullet.position.x > WINDOW_WIDTH) {
		return true;
	}
	if (bullet.position.y < 0) {
		return true;
	} else if (bullet.position.y > WINDOW_HEIGHT) {
		return true;
	}

	int x = bullet.position.x;
	int y = bullet.position.y;
	int r = 10;
	render::circle(buffer, x, y, r);

	return false;
}

void updateAndRenderBullets(VideoBuffer* buffer, GameState* gameState) {
	for (int i = 0; i < gameState->lastPlayerIndex + 1; i++) {
		gameState->playerBullets[i].position =
				gameState->playerBullets[i].position
						+ gameState->playerBullets[i].velocity;
		bool remove = false;
		if (gameState->playerBullets[i].position.x < 0) {
			remove = true;
		} else if (gameState->playerBullets[i].position.x > WINDOW_WIDTH) {
			remove = true;
		}
		if (gameState->playerBullets[i].position.y < 0) {
			remove = true;
		} else if (gameState->playerBullets[i].position.y > WINDOW_HEIGHT) {
			remove = true;
		}

		if (remove) {
			Bullet bullet = gameState->playerBullets[gameState->lastPlayerIndex];
			gameState->playerBullets[i] = bullet;
			gameState->lastPlayerIndex--;
			i--;
			if (i < 0) {
				break;
			}
		} else {
			int x = gameState->playerBullets[i].position.x;
			int y = gameState->playerBullets[i].position.y;
			int r = 10;
			render::circle(buffer, x, y, r);
		}
	}
}

void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory) {
	GameState *gameState = getGameState(memory);

	updateAndRenderRoad(buffer, gameState);

	updateAndRenderPlayer(buffer, input, gameState);

	updateAndRenderBullets(buffer, gameState);

//	Math::Vector2f position = {100, 100};
//	Math::Vector2f velocity = {1, 1};
//	spawnBullet(gameState, position, velocity, true);

	render::circle(buffer, 100, WINDOW_HEIGHT, 10);
	render::circle(buffer, 0, 100, 10);

	render::debugInformation(buffer, input, gameState);
}
