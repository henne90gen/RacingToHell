#include <math.h>
#include <sstream>
#include <string.h>

#include "RacingToHell.h"
#include "platform.h"
#include "Renderer.h"
#include "Font.h"
#include "Math.h"
#include "Sound.h"

#include "Helper.cpp"

void spawnTrafficCar(GameState* gameState);

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

	for (unsigned i = 0; i < NUM_PLAYER_TEXTURES; i++) {
		std::string carSprites = "./res/textures/cars/player"
				+ std::to_string(i) + ".bmp";
		File carFile = readFile(carSprites);
		gameState->resources.playerCarTextures[i] = readBmpIntoMemory(carFile,
				memory);
		freeFile(&carFile);
	}

	for (unsigned i = 0; i < NUM_TRAFFIC_TEXTURES; i++) {
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
	srand(time(0));

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

	gameState->trafficFrequency = 50;
	gameState->bulletFrequency = 50;
	gameState->bulletSpeed = 7;

	font::loadFont(memory, "./res/font/arial.ttf");

	Sound::loadWAV(memory, "./res/sound/shotAI.wav");

	loadTextures(memory, gameState);

	spawnTrafficCar(gameState);
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
		if (gameState->lastPlayerBulletIndex + 1 < arrSize) {

			bullet.color = (255 << 24) + (255 << 16);
			gameState->lastPlayerBulletIndex++;
			gameState->playerBullets[gameState->lastPlayerBulletIndex] = bullet;
		}
	} else {
		int arrSize = sizeof(gameState->aiBullets) / sizeof(Bullet);
		if (gameState->lastAIBulletIndex + 1 < arrSize) {
			gameState->lastAIBulletIndex++;
			gameState->aiBullets[gameState->lastAIBulletIndex] = bullet;
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
	render::circle(buffer, x, y, r, bullet.color);
	return false;
}

void updateAndRenderBullets(VideoBuffer* buffer, GameState* gameState) {
	if (gameState->frameCounter % gameState->bulletFrequency == 0) {
		int carIndex = std::rand() % gameState->lastTrafficCarIndex;
		TrafficCar car = gameState->traffic[carIndex];
		Math::Vector2f position = { car.position.x, car.position.y };

		Math::Vector2f velocity = gameState->player.position - car.position;
		velocity = velocity * (1.0f / Math::length(velocity));
		velocity = velocity * gameState->bulletSpeed;
		spawnBullet(gameState, position, velocity, false);
	}

	for (int i = 0; i < gameState->lastAIBulletIndex + 1; i++) {
		if (updateAndRenderBullet(buffer, gameState->aiBullets[i])) {
			Bullet bullet = gameState->aiBullets[gameState->lastAIBulletIndex];
			gameState->aiBullets[i] = bullet;
			gameState->lastAIBulletIndex--;
			i--;
			if (i < 0) {
				break;
			}
		}
	}

	for (int i = 0; i < gameState->lastPlayerBulletIndex + 1; i++) {
		if (updateAndRenderBullet(buffer, gameState->playerBullets[i])) {
			Bullet bullet =
					gameState->playerBullets[gameState->lastPlayerBulletIndex];
			gameState->playerBullets[i] = bullet;
			gameState->lastPlayerBulletIndex--;
			i--;
			if (i < 0) {
				break;
			}
		}
	}
}

void outputSound(GameState *state, SoundOutputBuffer *buffer, int toneHz) {
	int16_t toneVolume = 4000;
	int wavePeriod = buffer->samplesPerSecond / toneHz;
	int16_t *sampleOut = buffer->samples;
	for (int sampleIndex = 0; sampleIndex < buffer->sampleCount;
			sampleIndex++) {
		float sineValue = sinf(state->tSine);
		uint16_t sampleValue = (uint16_t) (sineValue * toneVolume);
		*sampleOut++ = sampleValue;
		*sampleOut++ = sampleValue;
		state->tSine += 2.0f * PI * 1.0f / (float) wavePeriod;
		if (state->tSine > 2.0f * PI)
			state->tSine -= 2.0f * PI;
	}
}

void getSoundSamples(GameMemory *memory, SoundOutputBuffer *soundBuffer) {
	GameState *gameState = getGameState(memory);

	outputSound(gameState, soundBuffer, 400);
}

void spawnTrafficCar(GameState* gameState) {
	TrafficCar car = { };
	car.carIndex = std::rand() % NUM_TRAFFIC_TEXTURES;
	float x = (std::rand() % 4) * (WINDOW_WIDTH / 4) + WINDOW_WIDTH / 8;
	car.position = (Math::Vector2f ) { x, -80 };
	car.speed = 5;

	unsigned arrSize = sizeof(gameState->traffic) / sizeof(TrafficCar);
	if (gameState->lastTrafficCarIndex + 1 < (int) arrSize) {
		gameState->lastTrafficCarIndex++;
		gameState->traffic[gameState->lastTrafficCarIndex] = car;
	}
}

void updateAndRenderTraffic(VideoBuffer* buffer, GameState *gameState) {
	if (gameState->frameCounter % gameState->trafficFrequency == 0) {
		spawnTrafficCar(gameState);
	}

	for (int i = 0; i < gameState->lastTrafficCarIndex + 1; i++) {
		TrafficCar *car = &gameState->traffic[i];
		car->position = car->position
				+ (Math::Vector2f ) { 0, (float) car->speed };

		if (car->position.y > WINDOW_HEIGHT) {
			gameState->traffic[i] =
					gameState->traffic[gameState->lastTrafficCarIndex];
			gameState->lastTrafficCarIndex--;
			i--;
			if (i < 0) {
				break;
			}
			continue;
		}

		Texture *texture =
				&gameState->resources.trafficCarTextures[car->carIndex];
		int x = car->position.x - texture->width / 2;
		int y = car->position.y - texture->height / 2;
		render::textureAlpha(buffer, texture, x, y);
	}
}

void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory) {
	GameState *gameState = getGameState(memory);
	gameState->frameCounter++;

	updateAndRenderRoad(buffer, gameState);

	updateAndRenderTraffic(buffer, gameState);

	updateAndRenderPlayer(buffer, input, gameState);

	updateAndRenderBullets(buffer, gameState);

	render::debugInformation(buffer, input, gameState);
}
