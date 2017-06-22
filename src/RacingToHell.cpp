#include "RacingToHell.h"

#include "MyMath.cpp"
#include "Memory.cpp"
#include "Collision.cpp"
#include "Renderer.cpp"
#include "Font.cpp"
#include "Sound.cpp"
#include "GameMenu.cpp"
#include "Helper.cpp"
#include "Init.cpp"

GameState *getGameState(GameMemory *memory) {
	if (!memory->isInitialized) {
		init(memory);
	}

	return (GameState *) (memory->permanent);
}

float getRoadSpeed(GameState *gameState) {
	// FIXME balance road speed
	return (float) (gameState->level) * 0.5f + 3.0f;
}

Render::Texture *getCurrentRoad(GameState *gameState) {
	return &gameState->resources.roadTextures[gameState->level % 4];
}

Render::Texture *getPlayerTexture(GameState *gameState) {
	return &gameState->resources.playerCarTextures[gameState->player.carIndex];
}

void updateAndRenderRoad(VideoBuffer *buffer, GameState *gameState,
		bool shouldUpdate) {
	if (shouldUpdate) {
		gameState->roadPosition += getRoadSpeed(gameState);
		if (gameState->roadPosition >= 800) {
			gameState->roadPosition = 0;
		}
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
			bullet.color = 0xff0000ff;
			gameState->lastPlayerBulletIndex++;
			gameState->playerBullets[gameState->lastPlayerBulletIndex] = bullet;
		}
	} else {
		int arrSize = sizeof(gameState->aiBullets) / sizeof(Bullet);
		if (gameState->lastAIBulletIndex + 1 < arrSize) {
			bullet.color = 0xff00ff00;
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
	if (input->downKeyPressed) {
		y += gameState->player.speed;
	}
	if (input->upKeyPressed) {
		y -= gameState->player.speed;
	}
	if (input->leftKeyPressed) {
		x -= gameState->player.speed;
	}
	if (input->rightKeyPressed) {
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
		// TODO play a nice animation before doing a game over
		gameOver(gameState);
	}

	// health
	if (gameState->player.health <= 0) {
		// TODO play the explosion before going game over
		gameOver(gameState);
	}

	// shooting
	if (input->shootKeyClicked
			&& gameState->player.energy >= 15/*FIXME make this a variable*/) {
		Math::Vector2f velocity = input->mousePosition
				- gameState->player.position;
		spawnBullet(gameState, gameState->player.position, velocity, true);
		// FIXME make this a variable
		gameState->player.energy -= 10;
	}

	Render::Texture *texture = getPlayerTexture(gameState);
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
	Render::Texture *texture = getPlayerTexture(gameState);

	int textureX = gameState->player.position.x - texture->width / 2;
	int textureY = gameState->player.position.y - texture->height / 2;

	Render::textureAlpha(buffer, texture, textureX, textureY);
}

bool updateAndRenderBullet(VideoBuffer *buffer, GameState *gameState,
		Bullet &bullet, bool isPlayerBullet, bool shouldUpdate) {

	if (shouldUpdate) {
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
				Render::Texture trafficTexture =
						gameState->resources.trafficCarTextures[car->carIndex];

				// decreases the size of the collision box
				int bufferZone = 17;
				Math::Rectangle trafficRect = getBoundingBox(car->position,
						trafficTexture.width - bufferZone,
						trafficTexture.height - bufferZone);

				Math::Rectangle collisionBox = getCollisionBox(trafficRect,
						bulletRect);

#if COLLISION_DEBUG
				Render::rectangle(buffer, collisionBox, 0x40ff0000);
#endif

				if (Collision::rectangle(collisionBox, bullet.position)) {
					// FIXME balance damage
					car->health -= 1;
					return true;
				}
			}
		} else {
			Render::Texture *playerTexture = getPlayerTexture(gameState);
			Math::Rectangle playerRect = getBoundingBox(
					gameState->player.position, playerTexture->width,
					playerTexture->height);
			Math::Rectangle collisionBox = getCollisionBox(playerRect,
					bulletRect);

#if COLLISION_DEBUG
			Render::rectangle(buffer, collisionBox, 0x40ff0000);
#endif

			if (Collision::rectangle(collisionBox, bullet.position)) {
				// FIXME balance damage
//				gameState->player.health -= 1;
				return true;
			}
		}
	}

	Render::circle(buffer, bullet.position, bullet.radius, bullet.color);
	return false;
}

void updateAndRenderBullets(VideoBuffer *buffer, GameState *gameState,
		bool shouldUpdate) {
	for (int i = 0; i < gameState->lastAIBulletIndex + 1; i++) {
		if (updateAndRenderBullet(buffer, gameState, gameState->aiBullets[i],
				false, shouldUpdate)) {
			removeElement(gameState->aiBullets, &gameState->lastAIBulletIndex,
					&i);
		}
	}

	for (int i = 0; i < gameState->lastPlayerBulletIndex + 1; i++) {
		if (updateAndRenderBullet(buffer, gameState,
				gameState->playerBullets[i], true, shouldUpdate)) {
			removeElement(gameState->playerBullets,
					&gameState->lastPlayerBulletIndex, &i);
		}
	}
}

void spawnTrafficCar(GameState *gameState) {
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

void updateAndRenderTraffic(VideoBuffer *buffer, GameState *gameState,
		bool shouldUpdate) {
	if (shouldUpdate
			&& gameState->frameCounter % gameState->trafficFrequency == 0) {
		spawnTrafficCar(gameState);
	}

	if (shouldUpdate
			&& gameState->frameCounter % gameState->bulletFrequency == 0
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
		Render::Texture *texture =
				&gameState->resources.trafficCarTextures[car->carIndex];

		if (shouldUpdate) {
			car->position = {car->position.x, ((float) car->speed) + car->position.y};

			if (car->health <= 0) {
				// TODO should we show an explosion?
				removeElement(gameState->traffic, &gameState->lastTrafficCarIndex,
						&i);
				continue;
			}

			if (car->position.y - texture->height / 2 > WINDOW_HEIGHT) {
				removeElement(gameState->traffic, &gameState->lastTrafficCarIndex,
						&i);
				continue;
			}

			Math::Rectangle carRect = getBoundingBox(car->position, texture->width,
					texture->height);

			Render::Texture *playerTexture = getPlayerTexture(gameState);
			Math::Rectangle playerRect = getBoundingBox(gameState->player.position,
					playerTexture->width,
					playerTexture->height);

			Math::Rectangle collisionBox = getCollisionBox(carRect, playerRect);

#if COLLISION_DEBUG
			Render::rectangle(buffer, collisionBox, 0x4000ff00);
#endif

			if (Collision::rectangle(collisionBox, gameState->player.position)) {
				gameOver(gameState);
			}
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

	unsigned arrSize = sizeof(gameState->items) / sizeof(Item);
	if (gameState->lastItemIndex + 1 < (int) arrSize) {
		gameState->lastItemIndex++;
		gameState->items[gameState->lastItemIndex] = item;
	}
}

void updateAndRenderItems(VideoBuffer *buffer, GameState *gameState,
		bool shouldUpdate) {
	if (shouldUpdate
			&& gameState->frameCounter % gameState->itemFrequency == 0) {
		spawnItem(gameState);
	}

	for (int i = 0; i < gameState->lastItemIndex + 1; i++) {
		Item *item = &gameState->items[i];
		Render::Texture *texture = &gameState->resources.itemTextures[item->itemIndex];

		if (shouldUpdate) {
			item->position = {item->position.x, (float) (item->position.y + getRoadSpeed(gameState))};

			// decreases the size of the collision box
			int bufferZone = 10;
			Math::Rectangle rect = getBoundingBox(item->position,
					texture->width - bufferZone,
					texture->height - bufferZone);

			Render::Texture *playerTexture = getPlayerTexture(gameState);
			Math::Rectangle playerRect = getBoundingBox(gameState->player.position,
					playerTexture->width,
					playerTexture->height);

			Math::Rectangle collisionRect = getCollisionBox(rect, playerRect);

#if COLLISION_DEBUG
			Render::rectangle(buffer, collisionRect, 0x00ffff50);
#endif

			if (Collision::rectangle(collisionRect, gameState->player.position)) {
				// TODO balance item effects
				switch (item->itemIndex) {
					case TOOLBOX_ID:
					// TODO test: percentage of maxHealth vs absolute value
					gameState->player.health += 5;
					if (gameState->player.health > gameState->player.maxHealth) {
						gameState->player.health = gameState->player.maxHealth;
					}
					break;
					case CANISTER_ID:
					// TODO test: percentage of maxEnergy vs absolute value
					gameState->player.energy += 100;

					if (gameState->player.energy > gameState->player.maxEnergy) {
						gameState->player.energy = gameState->player.maxEnergy;
					}
					break;
				}
				removeElement(gameState->items, &gameState->lastItemIndex, &i);
				continue;
			}

			if (item->position.y - texture->height / 2 > WINDOW_HEIGHT) {
				removeElement(gameState->items, &gameState->lastItemIndex, &i);
				continue;
			}
		}

		int x = item->position.x - texture->width / 2;
		int y = item->position.y - texture->height / 2;
		Render::textureAlpha(buffer, texture, x, y);
	}
}

void updateAndRenderTimer(VideoBuffer *buffer, GameState *gameState,
		bool shouldUpdate) {
	if (shouldUpdate) {
		gameState->levelTime += 10.0f / 60.0f;
		if (gameState->levelTime >= gameState->maxLevelTime) {
			gameState->levelTime = 0;
		}
	}

	Math::Rectangle rect = { };
	rect.position = {0, 0};
	rect.height = 10;
	rect.width = gameState->levelTime * WINDOW_WIDTH / gameState->maxLevelTime;
	Render::rectangle(buffer, rect, 0x80ffffff);
}

void updateAndRenderUI(VideoBuffer *buffer, GameState *gameState,
		bool shouldUpdate) {
	updateAndRenderTimer(buffer, gameState, shouldUpdate);

	static int widthWhenFull = 250;
	static uint32_t energyColor = 0xFFA51795;
	static uint32_t healthColor = 0xFFA51795;

	Math::Rectangle bar;
	bar.height = 20;

	// energy
	bar.width = gameState->player.energy * widthWhenFull
			/ gameState->player.maxEnergy;
	bar.position = {0, (float) (WINDOW_HEIGHT - bar.height)};
	Render::rectangle(buffer, bar, energyColor);

	// health
	bar.width = gameState->player.health * widthWhenFull
			/ gameState->player.maxHealth;
	bar.position = {0, (float) (WINDOW_HEIGHT - bar.height * 2)};
	Render::rectangle(buffer, bar, healthColor);
}

void updateAndRenderGame(VideoBuffer *buffer, Input *input, GameMemory *memory,
		GameState *gameState, bool update) {
	updateAndRenderRoad(buffer, gameState, update);

	// update player before doing any collision detection
	if (update) {
		updatePlayer(input, gameState);
	}

	updateAndRenderItems(buffer, gameState, update);

	updateAndRenderTraffic(buffer, gameState, update);

	// render player after traffic, so he is always on top
	renderPlayer(buffer, gameState);

	updateAndRenderBullets(buffer, gameState, update);

	updateAndRenderUI(buffer, gameState, update);
}

extern "C"
UPDATE_AND_RENDER(updateAndRender) {
	checkInputForClicks(input);

	GameState *gameState = getGameState(memory);
	gameState->frameCounter++;

	if (input->escapeKeyClicked) {
		handleMenuEscape(memory, gameState);
	}

	updateAndRenderGame(buffer, input, memory, gameState,
			gameState->currentMenu.state == GAME);
	if (gameState->currentMenu.state != GAME) {
		updateAndRenderMenu(memory, buffer, input, gameState);
	}

    Text::renderTextColored(memory, buffer, std::string("Test"), Math::Vector2f({ 0, 0 }), 20, 255, 255, 255);
    //Text::renderCharacterAlpha(memory, buffer, 'a', 100, 200, 255, 255, 255, 20);
}
