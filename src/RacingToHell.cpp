#include "RacingToHell.h"

#include "Helper.cpp"
#include "MyMath.cpp"
#include "Memory.cpp"
#include "Collision.cpp"
#include "Renderer.cpp"
#include "Sound.cpp"
#include "GameMenu.cpp"
#include "Boss.cpp"
#include "Init.cpp"

/**
 * Retrieves the GameState from memory
 */
GameState *getGameState(GameMemory *memory) {
	if (!memory->isInitialized) {
		init(memory);
	}
	return (GameState *) (memory->permanent);
}

/**
 * Calculates the current road speed from level (and soon difficulty)
 */
float getRoadSpeed(GameState *gameState) {
	// FIXME balance road speed
	return (float) (gameState->level) * 0.5f + 3.0f;
}

/**
 * Retrieves a pointer to the current road texture
 */
Render::Texture *getCurrentRoad(GameState *gameState) {
	return &gameState->resources.roadTextures[gameState->level % 4];
}

/**
 * Retrieves a pointer to the current player car texture
 */
Render::Texture *getPlayerTexture(GameState *gameState) {
	return &gameState->resources.playerCarTextures[gameState->player.carIndex];
}

/**
 * Updates and renders the road in the background
 */
void updateAndRenderRoad(GameMemory *memory, bool shouldUpdate) {
	GameState *gameState = getGameState(memory);

	for (unsigned y = 0; y < gameState->world.height; y++) {
		for (unsigned x = 0; x < gameState->world.width; x++) {
			Tile *tile = &gameState->world.tiles[y * gameState->world.width + x];
			uint32_t color = 0xff0000ff;
			if (tile->traversable) {
				color = 0x00ff00ff;
			} else {
				checkPlayerTileCollision(&gameState->player, tile);
			}
//			printf("X: %f, Y: %f, Size: %f\n", rect.position.x, rect.position.y,
//					rect.size.x);
			Render::rectangle(memory, tile->rect, color);
		}
	}
}

/**
 * Spawns a bullet at the location of a randomly chosen car
 */
void spawnBullet(GameState *gameState, Math::Vector2f position,
		Math::Vector2f velocity, bool playerBullet) {

	velocity = Math::normalize(velocity);
	// FIXME balance bullet speed
	velocity = velocity * gameState->bulletSpeed;

	Bullet bullet = { };
	bullet.position = position;
	bullet.velocity = velocity;
	// FIXME balance bullet radius
	bullet.radius = 0.05;

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
		Sound::output(gameState, &gameState->resources.aiShot, 0.1f, 0.1f,
				Sound::PLAY_ONCE);
	}
}

/**
 * Updates the following aspects of the player:
 * 		- health
 * 		- energy
 * 		- position according to input
 * 		- keeping him on screen
 * 		- shooting
 */
void updatePlayer(Input *input, GameState *gameState) {
	float speed = 0;
	float direction = 0;
	if (input->downKeyPressed) {
		speed -= 0.001;
	}
	if (input->upKeyPressed) {
		speed += 0.001;
	}
	if (input->leftKeyPressed) {
		direction += 0.09;
	}
	if (input->rightKeyPressed) {
		direction -= 0.09;
	}

	// movement
	gameState->player.speed += speed;
	if (gameState->player.speed > PLAYER_SPEED) {
		gameState->player.speed = PLAYER_SPEED;
	} else if (gameState->player.speed < -PLAYER_SPEED) {
		gameState->player.speed = -PLAYER_SPEED;
	}
	gameState->player.direction = Math::normalize(
			Math::rotate(gameState->player.direction, direction));
	gameState->player.position = gameState->player.position
			+ gameState->player.direction * gameState->player.speed;
//	printf("Player position: %f, %f\n", gameState->player.position.x,
//			gameState->player.position.y);
//	printf("Player direction: %f, %f\n", gameState->player.direction.x,
//			gameState->player.direction.y);

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
	/*&& gameState->player.energy >= 15*//*FIXME make this a variable*/) {
		Math::Vector2f velocity = input->mousePosition
				- gameState->player.position;
		spawnBullet(gameState, gameState->player.position, velocity, true);
		// FIXME make this a variable
		gameState->player.energy -= 10;
	}

	Render::Texture *texture = getPlayerTexture(gameState);
	// checking left and right
	if (gameState->player.position.x < texture->width / 2) {
//		gameState->player.position.x = texture->width / 2;
	} else if (gameState->player.position.x
			> WINDOW_WIDTH - texture->width / 2) {
//		gameState->player.position.x = WINDOW_WIDTH - texture->width / 2;
	}

	// checking top and bottom
	if (gameState->player.position.y < texture->height / 2) {
//		gameState->player.position.y = texture->height / 2;
	} else if (gameState->player.position.y
			> WINDOW_HEIGHT - texture->height / 2) {
//		gameState->player.position.y = WINDOW_HEIGHT - texture->height / 2;
	}
}

/**
 * Renders the player to the video buffer
 */
void renderPlayer(GameMemory *memory, GameState *gameState) {
	Render::texture(memory, getPlayerTexture(gameState),
			gameState->player.position, gameState->player.size,
			gameState->player.direction);
}

/**
 * Updates and renders a single bullet
 * Checks for collisions with other cars or the player
 */
bool updateAndRenderBullet(GameMemory *memory, GameState *gameState,
		Bullet &bullet, bool isPlayerBullet, bool shouldUpdate) {
	if (shouldUpdate) {
		bullet.position = bullet.position + bullet.velocity;

		if (bullet.position.x < -10.0) {
			return true;
		} else if (bullet.position.x > 10.0) {
			return true;
		}
		if (bullet.position.y < -10.0) {
			return true;
		} else if (bullet.position.y > 10.0) {
			return true;
		}

		Math::Rectangle bulletRect = getBoundingBox(bullet.position,
				bullet.radius * 2, bullet.radius * 2);

		if (isPlayerBullet) {
			for (int i = 0; i < gameState->world.lastTrafficCarIndex + 1; i++) {
				Car *car = &gameState->world.traffic[i];
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

	Render::circle(memory, bullet.position, bullet.radius, bullet.color);
	return false;
}

/**
 * Updates and renders all bullets (player and AI)
 */
void updateAndRenderBullets(GameMemory *memory, GameState *gameState,
		bool shouldUpdate) {
	for (int i = 0; i < gameState->lastAIBulletIndex + 1; i++) {
		if (updateAndRenderBullet(memory, gameState, gameState->aiBullets[i],
				false, shouldUpdate)) {
			removeElement(gameState->aiBullets, &gameState->lastAIBulletIndex,
					&i);
		}
	}

	for (int i = 0; i < gameState->lastPlayerBulletIndex + 1; i++) {
		if (updateAndRenderBullet(memory, gameState,
				gameState->playerBullets[i], true, shouldUpdate)) {
			removeElement(gameState->playerBullets,
					&gameState->lastPlayerBulletIndex, &i);
		}
	}
}

/**
 * Spawns a new car on one of the four lanes
 */
void spawnTrafficCar(GameState *gameState) {
	Car car = { };
	car.carIndex = std::rand() % NUM_TRAFFIC_TEXTURES;
	float x = (std::rand() % 4) * (WINDOW_WIDTH / 4) + WINDOW_WIDTH / 8;
	car.position = {x, -80};
	car.speed = 5;
	car.health = 75;

	unsigned arrSize = sizeof(gameState->world.traffic) / sizeof(Car);
	if (gameState->world.lastTrafficCarIndex + 1 < (int) arrSize) {
		gameState->world.lastTrafficCarIndex++;
		gameState->world.traffic[gameState->world.lastTrafficCarIndex] = car;
	}
}

/**
 * Updates and renders all cars
 * Checks for collision between one of the cars and the player
 */
void updateAndRenderTraffic(VideoBuffer *buffer, GameState *gameState,
		bool shouldUpdate) {
	if (shouldUpdate
			&& gameState->frameCounter % gameState->trafficFrequency == 0) {
		spawnTrafficCar(gameState);
	}

	if (shouldUpdate
			&& gameState->frameCounter % gameState->bulletFrequency == 0
			&& gameState->world.lastTrafficCarIndex >= 0) {
		// FIXME maybe choose the car that is furthest away from the player
		int carIndex = 0;
		if (gameState->world.lastTrafficCarIndex > 0) {
			carIndex = std::rand() % gameState->world.lastTrafficCarIndex;
		}
		Car car = gameState->world.traffic[carIndex];
		shootAtPlayer(gameState, car.position);
	}

	for (int i = 0; i < gameState->world.lastTrafficCarIndex + 1; i++) {
		Car *car = &gameState->world.traffic[i];
		Render::Texture *texture =
				&gameState->resources.trafficCarTextures[car->carIndex];

		if (shouldUpdate) {
			car->position = {car->position.x, ((float) car->speed) + car->position.y};

			if (car->health <= 0) {
				// TODO should we show an explosion?
				removeElement(gameState->world.traffic, &gameState->world.lastTrafficCarIndex,
						&i);
				continue;
			}

			if (car->position.y - texture->height / 2 > WINDOW_HEIGHT) {
				removeElement(gameState->world.traffic, &gameState->world.lastTrafficCarIndex,
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

//		int x = car->position.x - texture->width / 2;
		int y = car->position.y - texture->height / 2;
//		Render::textureAlpha(buffer, texture, x, y);

		Render::bar(buffer, { car->position.x, (float) y - 13 }, car->health,
				0xff0000ff);
	}
}

/**
 * Spawns a random item on one of the four lanes
 */
void spawnItem(GameState *gameState) {
	Item item = { };
	item.itemIndex = std::rand() % NUM_ITEM_TEXTURES;
	float x = (std::rand() % 4) * (WINDOW_WIDTH / 4) + WINDOW_WIDTH / 8;
	item.position = {x, -80};

	unsigned arrSize = sizeof(gameState->world.items) / sizeof(Item);
	if (gameState->world.lastItemIndex + 1 < (int) arrSize) {
		gameState->world.lastItemIndex++;
		gameState->world.items[gameState->world.lastItemIndex] = item;
	}
}

/**
 * Updates and renders all items
 * Checks for collision with player and handles pickup effects
 */
void updateAndRenderItems(VideoBuffer *buffer, GameState *gameState,
		bool shouldUpdate) {
	if (shouldUpdate
			&& gameState->frameCounter % gameState->itemFrequency == 0) {
		spawnItem(gameState);
	}

	for (int i = 0; i < gameState->world.lastItemIndex + 1; i++) {
		Item *item = &gameState->world.items[i];
		Render::Texture *texture =
				&gameState->resources.itemTextures[item->itemIndex];

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
				removeElement(gameState->world.items, &gameState->world.lastItemIndex, &i);
				continue;
			}

			if (item->position.y - texture->height / 2 > WINDOW_HEIGHT) {
				removeElement(gameState->world.items, &gameState->world.lastItemIndex, &i);
				continue;
			}
		}

//		int x = item->position.x - texture->width / 2;
//		int y = item->position.y - texture->height / 2;
//		Render::textureAlpha(buffer, texture, x, y);
	}
}

/**
 * Updates and renders the UI with player health, player energy and timer
 */
void updateAndRenderUI(GameMemory *memory, bool shouldUpdate) {
	GameState *gameState = getGameState(memory);

	static uint32_t energyColor = 0xFFA51795;
	static uint32_t healthColor = 0xFFA51795;

	float barWidth = 0.05;
	float screenHeight = 2.0;
	float screenWidth = memory->aspectRatio * 2.0;

	// energy
	Math::Rectangle energyBar;
	float energyHeight = gameState->player.energy * screenHeight / 2.0
			/ gameState->player.maxEnergy;
	energyBar.size = Math::Vector2f(barWidth, energyHeight);
	energyBar.position = Math::Vector2f(-screenWidth / 2.0, energyHeight - 1.0);
	Render::rectangle(memory, energyBar, energyColor);

	// health
	Math::Rectangle healthBar;
	float healthHeight = gameState->player.health * screenHeight / 2.0
			/ gameState->player.maxHealth;
	healthBar.size = Math::Vector2f(barWidth, healthHeight);
	healthBar.position = Math::Vector2f(-screenWidth / 2.0 + barWidth,
			healthHeight - 1.0);
	Render::rectangle(memory, healthBar, healthColor);
}

/**
 * Updates and renders the game with all its entities
 */
void updateAndRenderGame(Input *input, GameMemory *memory, GameState *gameState,
		bool update) {

// update player before doing any collision detection
	if (update) {
		updatePlayer(input, gameState);
	}

	updateAndRenderRoad(memory, update);

//	updateAndRenderItems(buffer, gameState, update);

//	updateAndRenderTraffic(buffer, gameState, update);

	if (gameState->isInBossFight) {
//		updateAndRenderBoss(buffer, gameState, update);
	}

	if (gameState->menuState != MenuState::CREDITS) {
		// render player after traffic, so he is always on top
		renderPlayer(memory, gameState);
	}

	updateAndRenderBullets(memory, gameState, update);

	if (gameState->menuState != MenuState::MAIN
			&& gameState->menuState != MenuState::CREDITS) {
		updateAndRenderUI(memory, update);
	}
}

extern "C"
UPDATE_AND_RENDER(updateAndRender) {
	GameState *gameState = beginFrame(memory, input);
	if (input->enterKeyClicked) {
		if (!strcmp(gameState->resources.fontName, "arial")) {
			loadFont(memory, "./res/font/DejaVuSansMono.ttf");
		} else {
			loadFont(memory, "./res/font/arial.ttf");
		}
	}

	updateAndRenderGame(input, memory, gameState,
			gameState->menuState == MenuState::GAME);

//	Math::Rectangle rect = { { -memory->aspectRatio, 1.0 }, { 2
//			* memory->aspectRatio, 2.0 } };
//	Render::rectangle(memory, rect, 0xff00ff80);
//	Math::Vector2f p1 = {1.0, 1.0};
//	Math::Vector2f p2 = {1.0, -1.0};
//	Math::Vector2f p3 = {0.0, 0.0};
//	Render::triangle(memory, p1, p2, p3, 0xff00ff80);

	Math::Vector2f position = Math::Vector2f(-1.0f, 0);
//	Render::text(memory, "Franzi is the best", position,
//			Render::FontSize::Small, 0x0000ffff);
//	position = Math::Vector2f(-0.2, 0);
	Render::text(memory, "The quick brown fox jumps over the lazy dog",
			position, Render::FontSize::Medium, 0x0000ffff);
//	position = position + Math::Vector2f(0.6, 0);
//	Render::text(memory, "Franzi is the best", position,
//			Render::FontSize::Large, 0x0000ffff);

	if (input->escapeKeyClicked && gameState->menuState == MenuState::GAME) {
		loadMenu(gameState, MenuState::PAUSE);
	} else if (gameState->menuState != MenuState::GAME) {
		updateAndRenderMenus(memory, input);
	}
}
