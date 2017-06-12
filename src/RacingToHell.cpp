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

Texture* getPlayerTexture(GameState *gameState) {
	return &gameState->resources.playerCarTextures[gameState->player.carIndex];
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

void updatePlayer(Input *input, GameState *gameState) {
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

	// movement
	if (x || y) {
		Math::Vector2f movement = { };
		movement.x = x;
		movement.y = y;
		movement = Math::normalize(movement);
		movement = movement * gameState->player.speed;
		gameState->player.position = gameState->player.position + movement;
	}

	// energy
	gameState->player.energy -= 1;
	if (gameState->player.energy <= 0) {
		// TODO Game over
		gameState->player.energy = 100;
	}

	// health
	if (gameState->player.health <= 0) {
		// TODO Game over
		gameState->player.health = 100;
	}

	if (input->shootKeyClicked
			&& gameState->player.energy >= 15/*FIXME make this a variable*/) {
		Math::Vector2f velocity = input->mousePosition
				- gameState->player.position;
		spawnBullet(gameState, gameState->player.position, velocity, true);
		// FIXME make this a variable
		gameState->player.energy -= 10;
	}

	Texture *texture = getPlayerTexture(gameState);
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
}

void renderPlayer(VideoBuffer *buffer, GameState *gameState) {
	Texture *texture = getPlayerTexture(gameState);

	int textureX = gameState->player.position.x - texture->width / 2;
	int textureY = gameState->player.position.y - texture->height / 2;

	Render::textureAlpha(buffer, texture, textureX, textureY);

	Render::bar(buffer, { gameState->player.position.x, (float) textureY - 23 },
			gameState->player.health, 0xff0000ff);
	Render::bar(buffer, { gameState->player.position.x, (float) textureY - 13 },
			gameState->player.energy, 0x0000fffe);
}

bool updateAndRenderBullet(VideoBuffer* buffer, GameState* gameState,
		Bullet &bullet, bool isPlayerBullet) {
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

	if (isPlayerBullet) {
		for (int i = 0; i < gameState->lastTrafficCarIndex + 1; i++) {
			Car *car = &gameState->traffic[i];
			Texture trafficTexture =
					gameState->resources.trafficCarTextures[car->carIndex];

			// decreases the size of the collision box
			int bufferZone = 17;
			Math::Rectangle trafficRect = getBoundingBox(car->position,
					trafficTexture.width - bufferZone,
					trafficTexture.height - bufferZone);

			Math::Rectangle collisionBox = getCollisionBox(trafficRect,
					bulletRect);

#if COLLISION_DEBUG
			Render::rectangle(buffer, collisionBox, 0xff000040);
#endif

			if (Collision::rectangle(collisionBox, bullet.position)) {
				// FIXME balance damage
				car->health -= 1;
				return true;
			}
		}
	} else {
		Texture *playerTexture = getPlayerTexture(gameState);
		Math::Rectangle playerRect = getBoundingBox(gameState->player.position,
				playerTexture->width, playerTexture->height);
		Math::Rectangle collisionBox = getCollisionBox(playerRect, bulletRect);

#if COLLISION_DEBUG
		Render::rectangle(buffer, collisionBox, 0xff000040);
#endif

		if (Collision::rectangle(collisionBox, bullet.position)) {
			// FIXME balance damage
			gameState->player.health -= 1;
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
	Car car = { };
	car.carIndex = std::rand() % NUM_TRAFFIC_TEXTURES;
	float x = (std::rand() % 4) * (WINDOW_WIDTH / 4) + WINDOW_WIDTH / 8;
	car.position = {x, -80};
	car.speed = 5;
	car.health = 75;

	unsigned arrSize = sizeof(gameState->traffic) / sizeof(Car);
	if (gameState->lastTrafficCarIndex + 1 < (int) arrSize) {
		gameState->lastTrafficCarIndex++;
		gameState->traffic[gameState->lastTrafficCarIndex] = car;
	}
}

void updateAndRenderTraffic(VideoBuffer* buffer, GameState *gameState) {
	if (gameState->frameCounter % gameState->trafficFrequency == 0) {
		spawnTrafficCar(gameState);
	}

	if (gameState->frameCounter % gameState->bulletFrequency == 0
			&& gameState->lastTrafficCarIndex >= 0) {
		// FIXME maybe choose the car that is furthest away from the player
		int carIndex = 0;
		if (gameState->lastTrafficCarIndex > 0) {
			carIndex = std::rand() % gameState->lastTrafficCarIndex;
		}
		Car car = gameState->traffic[carIndex];
		Math::Vector2f position = { car.position.x, car.position.y };
		Math::Vector2f velocity = gameState->player.position - car.position;
		spawnBullet(gameState, position, velocity, false);
	}

	for (int i = 0; i < gameState->lastTrafficCarIndex + 1; i++) {
		Car *car = &gameState->traffic[i];
		car->position = {car->position.x, ((float) car->speed) + car->position.y};

		Texture *texture =
				&gameState->resources.trafficCarTextures[car->carIndex];

		if (car->health <= 0) {
			// TODO should we show an explosion?
			gameState->traffic[i] =
					gameState->traffic[gameState->lastTrafficCarIndex];
			gameState->lastTrafficCarIndex--;
			i--;
			continue;
		}

		if (car->position.y - texture->height / 2 > WINDOW_HEIGHT) {
			gameState->traffic[i] =
					gameState->traffic[gameState->lastTrafficCarIndex];
			gameState->lastTrafficCarIndex--;
			i--;
			continue;
		}

		Math::Rectangle carRect = getBoundingBox(car->position, texture->width,
				texture->height);

		Texture *playerTexture = getPlayerTexture(gameState);
		Math::Rectangle playerRect = getBoundingBox(gameState->player.position,
				playerTexture->width, playerTexture->height);

		Math::Rectangle collisionBox = getCollisionBox(carRect, playerRect);

#if COLLISION_DEBUG
		Render::rectangle(buffer, collisionBox, 0x0000ff40);
#endif

		if (Collision::rectangle(collisionBox, gameState->player.position)) {
			// TODO Game over!
		}

		int x = car->position.x - texture->width / 2;
		int y = car->position.y - texture->height / 2;
		Render::textureAlpha(buffer, texture, x, y);

		Render::bar(buffer, { car->position.x, (float) y - 13 }, car->health,
				0xff0000ff);
	}
}

void spawnItem(GameState *gameState) {
	Item item = { };
	item.itemIndex = std::rand() % NUM_ITEM_TEXTURES;
	float x = (std::rand() % 4) * (WINDOW_WIDTH / 4) + WINDOW_WIDTH / 8;
	item.position = {x, -80};
	item.speed = getRoadSpeed(gameState);

	unsigned arrSize = sizeof(gameState->items) / sizeof(Item);
	if (gameState->lastItemIndex + 1 < (int) arrSize) {
		gameState->lastItemIndex++;
		gameState->items[gameState->lastItemIndex] = item;
	}
}

void updateAndRenderItems(VideoBuffer *buffer, GameState *gameState) {
	if (gameState->frameCounter % gameState->itemFrequency == 0) {
		spawnItem(gameState);
	}

	for (int i = 0; i < gameState->lastItemIndex + 1; i++) {
		Item *item = &gameState->items[i];
		item->position = {item->position.x, (float)(item->position.y + item->speed)};

		Texture *texture = &gameState->resources.itemTextures[item->itemIndex];

		// decreases the size of the collision box
		int bufferZone = 10;
		Math::Rectangle rect = getBoundingBox(item->position,
				texture->width - bufferZone, texture->height - bufferZone);

		Texture *playerTexture = getPlayerTexture(gameState);
		Math::Rectangle playerRect = getBoundingBox(gameState->player.position,
				playerTexture->width, playerTexture->height);

		Math::Rectangle collisionRect = getCollisionBox(rect, playerRect);

#if COLLISION_DEBUG
		Render::rectangle(buffer, collisionRect, 0x00ffff50);
#endif

		if (Collision::rectangle(collisionRect, gameState->player.position)) {
			switch (item->itemIndex) {
			case TOOLBOX_ID:
				gameState->player.health += 10;
				break;
			case CANISTER_ID:
				gameState->player.energy += 10;
				break;
			}
			gameState->items[i] = gameState->items[gameState->lastItemIndex];
			gameState->lastItemIndex--;
			i--;
			continue;
		}

		if (item->position.y - texture->height / 2 > WINDOW_HEIGHT) {
			gameState->items[i] = gameState->items[gameState->lastItemIndex];
			gameState->lastItemIndex--;
			i--;
			continue;
		}

		int x = item->position.x - texture->width / 2;
		int y = item->position.y - texture->height / 2;
		Render::textureAlpha(buffer, texture, x, y);
	}
}

void updateAndRenderTimer(VideoBuffer *buffer, GameState* gameState) {
	gameState->levelTime += 1.0f;
	if (gameState->levelTime >= WINDOW_WIDTH) {
		gameState->levelTime = 0;
	}

	Math::Rectangle rect = { };
	rect.position = {0, 0};
	rect.height = 10;
	rect.width = gameState->levelTime;
	Render::rectangle(buffer, rect, 0xfffffff0);
}

void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory) {
	GameState *gameState = getGameState(memory);
	gameState->frameCounter++;

	updateAndRenderRoad(buffer, gameState);

	// update player before doing any collision detection
	updatePlayer(input, gameState);

	updateAndRenderItems(buffer, gameState);

	updateAndRenderTraffic(buffer, gameState);

	// render player after traffic, so he is always on top
	renderPlayer(buffer, gameState);

	updateAndRenderBullets(buffer, gameState);

	updateAndRenderTimer(buffer, gameState);

//	Render::debugInformation(buffer, input, gameState);
//	Text::renderCharacterAlpha(buffer, 'a', 10, 10, 255, 0, 0, 20);
}
