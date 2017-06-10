#include <stdlib.h>
#include <cstdlib>
#include "RacingToHell.h"

void spawnTrafficCar(GameState *gameState);

#include "Helper.cpp"
#include "Init.cpp"

GameState* getGameState(GameMemory* memory) {
	if (!memory->isInitialized) {
		init(memory);
	}

	return (GameState *) (memory->permanent);
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
	Render::backgroundTexture(buffer, getCurrentRoad(gameState),
			gameState->roadPosition);
	Render::backgroundTexture(buffer, getCurrentRoad(gameState),
			gameState->roadPosition - 800);
}

void spawnBullet(GameState *gameState, Math::Vector2f position,
		Math::Vector2f velocity, bool playerBullet) {

	printf("Spawning bullet\n");

	velocity = velocity * (1.0 / Math::length(velocity));
	// FIXME balance bullet speed
	velocity = velocity * gameState->bulletSpeed;

	Bullet bullet = { };
	bullet.position = position;
	bullet.velocity = velocity;
	// FIXME balance bullet radius
	bullet.radius = 10;

	if (playerBullet) {
		int arrSize = sizeof(gameState->playerBullets) / sizeof(Bullet);
		if (gameState->lastPlayerBulletIndex + 1 < arrSize) {
			bullet.color = (255 << 24) + 255;
			gameState->lastPlayerBulletIndex++;
			gameState->playerBullets[gameState->lastPlayerBulletIndex] = bullet;
		}
	} else {
		int arrSize = sizeof(gameState->aiBullets) / sizeof(Bullet);
		if (gameState->lastAIBulletIndex + 1 < arrSize) {
			bullet.color = (255 << 24) + (255 << 8);
			gameState->lastAIBulletIndex++;
			gameState->aiBullets[gameState->lastAIBulletIndex] = bullet;
		}
	}

	if (playerBullet) {
		Sound::output(gameState, &gameState->resources.playerShot, 0.1f, 0.1f,
				Sound::PLAY_ONCE);
	} else {
		Sound::output(gameState, &gameState->resources.AIShot, 0.1f, 0.1f,
				Sound::PLAY_ONCE);
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

	if (input->shootKeyClicked) {
		Math::Vector2f velocity = input->mousePosition
				- gameState->player.position;
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
	Render::textureAlpha(buffer, texture, textureX, textureY);
}

bool updateAndRenderBullet(VideoBuffer* buffer, GameState* gameState,
		Bullet &bullet, bool playerBullet) {
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

	Math::Rectangle bulletRect = getBoundingBox(bullet.position,
			bullet.radius * 2, bullet.radius * 2);

	if (playerBullet) {
		for (int i = 0; i < gameState->lastTrafficCarIndex + 1; i++) {
			TrafficCar car = gameState->traffic[i];
			Texture trafficTexture =
					gameState->resources.trafficCarTextures[car.carIndex];
			Math::Rectangle trafficRect = getBoundingBox(car.position,
					trafficTexture.width, trafficTexture.height);
			Math::Rectangle collisionBox = getCollisionBox(trafficRect,
					bulletRect);

			Render::rectangle(buffer, collisionBox, 0xff000040);

			if (Collision::rectangle(collisionBox, bullet.position)) {
				// TODO car should take damage
				return true;
			}
		}
	} else {
		Texture playerTexture =
				gameState->resources.playerCarTextures[gameState->player.carIndex];
		Math::Rectangle playerRect = getBoundingBox(gameState->player.position,
				playerTexture.width, playerTexture.height);
		Math::Rectangle collisionBox = getCollisionBox(playerRect, bulletRect);
		Render::rectangle(buffer, collisionBox, 0xff000040);

		if (Collision::rectangle(collisionBox, bullet.position)) {
			// TODO player should take damage
			return true;
		}
	}

	Render::circle(buffer, bullet.position, bullet.radius, bullet.color);
	return false;
}

void updateAndRenderBullets(VideoBuffer* buffer, GameState* gameState) {
	for (int i = 0; i < gameState->lastAIBulletIndex + 1; i++) {
		if (updateAndRenderBullet(buffer, gameState, gameState->aiBullets[i],
				false)) {
			Bullet bullet = gameState->aiBullets[gameState->lastAIBulletIndex];
			gameState->aiBullets[i] = bullet;
			gameState->lastAIBulletIndex--;
			i--;
		}
	}

	for (int i = 0; i < gameState->lastPlayerBulletIndex + 1; i++) {
		if (updateAndRenderBullet(buffer, gameState,
				gameState->playerBullets[i], true)) {
			Bullet bullet =
					gameState->playerBullets[gameState->lastPlayerBulletIndex];
			gameState->playerBullets[i] = bullet;
			gameState->lastPlayerBulletIndex--;
			i--;
		}
	}
}

void spawnTrafficCar(GameState* gameState) {
	TrafficCar car = { };
	car.carIndex = std::rand() % NUM_TRAFFIC_TEXTURES;
	float x = (std::rand() % 4) * (WINDOW_WIDTH / 4) + WINDOW_WIDTH / 8;
	// FIXME set position back to -80 and enable speed again
	car.position = {x, 180};
//	car.speed = 5;

	unsigned arrSize = sizeof(gameState->traffic) / sizeof(TrafficCar);
	if (gameState->lastTrafficCarIndex + 1 < (int) arrSize) {
		gameState->lastTrafficCarIndex++;
		gameState->traffic[gameState->lastTrafficCarIndex] = car;
	}
}

void updateAndRenderTraffic(VideoBuffer* buffer, GameState *gameState) {
	if (gameState->frameCounter % gameState->trafficFrequency == 0) {
//		spawnTrafficCar(gameState);
	}

	if (gameState->frameCounter % gameState->bulletFrequency == 0
			&& gameState->lastTrafficCarIndex >= 0) {
		// FIXME maybe choose the car that is furthest away from the player
		int carIndex = 0;
		if (gameState->lastTrafficCarIndex > 0) {
			carIndex = std::rand() % gameState->lastTrafficCarIndex;
		}
		TrafficCar car = gameState->traffic[carIndex];
		Math::Vector2f position = { car.position.x, car.position.y };
		Math::Vector2f velocity = gameState->player.position - car.position;
		spawnBullet(gameState, position, velocity, false);
	}

	for (int i = 0; i < gameState->lastTrafficCarIndex + 1; i++) {
		TrafficCar *car = &gameState->traffic[i];
		car->position = {car->position.x, ((float) car->speed) + car->position.y};

		Texture *texture =
				&gameState->resources.trafficCarTextures[car->carIndex];
		Math::Rectangle carRect = getBoundingBox(car->position, texture->width,
				texture->height);

		Texture *playerTexture =
				&gameState->resources.playerCarTextures[gameState->player.carIndex];
		Math::Rectangle playerRect = getBoundingBox(gameState->player.position,
				playerTexture->width, playerTexture->height);

		Math::Rectangle collisionBox = getCollisionBox(playerRect, carRect);

		if (Collision::rectangle(collisionBox, car->position)) {
			printf("Traffic player collision!\n");
		}

		if (car->position.y - texture->height > WINDOW_HEIGHT) {
			gameState->traffic[i] =
					gameState->traffic[gameState->lastTrafficCarIndex];
			gameState->lastTrafficCarIndex--;
			i--;
			continue;
		}

		int x = car->position.x - texture->width / 2;
		int y = car->position.y - texture->height / 2;
		Render::textureAlpha(buffer, texture, x, y);
	}
}

void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory) {
	GameState *gameState = getGameState(memory);
	gameState->frameCounter++;

	updateAndRenderRoad(buffer, gameState);

	updateAndRenderTraffic(buffer, gameState);

	updateAndRenderPlayer(buffer, input, gameState);

	updateAndRenderBullets(buffer, gameState);

//	Render::debugInformation(buffer, input, gameState);

//	Text::renderCharacterAlpha(buffer, 'a', 10, 10, 255, 0, 0, 20);
}
