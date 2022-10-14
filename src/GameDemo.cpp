#include "GameDemo.h"

#include "Boss.h"
#include "GameMenu.h"
#include "Helper.h"
#include "Sound.h"

/**
 * Calculates the current road speed from level (and soon difficulty)
 */
float getRoadSpeed(GameState *gameState) {
    // FIXME balance road speed
    return ((float)(gameState->level) * 0.5f + 3.0f) * 0.0002F;
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
void updateAndRenderRoad(Platform &platform, bool shouldUpdate) {
    GameState *gameState = getGameState(platform);

    double roadHeight = 2.0 * (1.0 / platform.memory.aspectRatio);
    if (shouldUpdate) {
        gameState->roadOffset -= getRoadSpeed(gameState) * platform.frameTimeMs;
        if (gameState->roadOffset < -roadHeight) {
            gameState->roadOffset = 0.0F;
        }
    }

    Render::Texture *roadTexture = getCurrentRoad(gameState);
    {
        auto pos = glm::vec2(0.0, gameState->roadOffset);
        auto size = glm::vec2(2.0, roadHeight);
        auto direction = glm::vec2(0.0, 1.0);
        Render::pushTexture(gameState, roadTexture, pos, size, direction, AtomPlane::BACKGROUND);
    }

    {
        float otherRoadOffset = gameState->roadOffset + roadHeight;
        auto pos = glm::vec2(0.0, otherRoadOffset);
        auto size = glm::vec2(2.0, roadHeight);
        auto direction = glm::vec2(0.0, 1.0);
        Render::pushTexture(gameState, roadTexture, pos, size, direction, AtomPlane::BACKGROUND);
    }
}

/**
 * Spawns a bullet at the location of a randomly chosen car
 */
void spawnBullet(GameState *gameState, glm::vec2 position, glm::vec2 direction, bool playerBullet) {

    direction = Math::normalize(direction);
    // FIXME balance bullet speed
    direction = direction * gameState->bulletSpeed;

    Bullet bullet = {};
    bullet.position = position;
    bullet.velocity = direction;
    // FIXME balance bullet radius
    bullet.radius = 0.05;

    if (playerBullet) {
        int arrSize = sizeof(gameState->playerBullets) / sizeof(Bullet);
        if (gameState->nextPlayerBulletIndex < arrSize) {
            bullet.color = glm::vec4(1, 0, 0, 1);
            gameState->playerBullets[gameState->nextPlayerBulletIndex] = bullet;
            gameState->nextPlayerBulletIndex++;
        }
    } else {
        int arrSize = sizeof(gameState->aiBullets) / sizeof(Bullet);
        if (gameState->nextAIBulletIndex < arrSize) {
            bullet.color = glm::vec4(1, 0, 1, 1);
            gameState->aiBullets[gameState->nextAIBulletIndex] = bullet;
            gameState->nextAIBulletIndex++;
        }
    }

    if (playerBullet) {
        Sound::output(gameState, &gameState->resources.playerShot, 0.1f, 0.1f, Sound::PLAY_ONCE);
    } else {
        Sound::output(gameState, &gameState->resources.aiShot, 0.1f, 0.1f, Sound::PLAY_ONCE);
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
void updatePlayer(Platform &platform) {
    auto gameState = getGameState(platform);

    glm::vec2 movement = {};
    if (platform.input.leftKeyPressed) {
        movement.x = -1;
    }
    if (platform.input.rightKeyPressed) {
        movement.x = 1;
    }
    if (platform.input.leftKeyPressed && platform.input.rightKeyPressed) {
        movement.x = 0;
    }

    if (platform.input.upKeyPressed) {
        movement.y = 1;
    }
    if (platform.input.downKeyPressed) {
        movement.y = -1;
    }
    if (platform.input.upKeyPressed && platform.input.downKeyPressed) {
        movement.y = 0;
    }

    // movement
    gameState->player.position += movement * gameState->player.speed * float(platform.frameTimeMs) * 0.1F;

    // energy
    gameState->player.fuel -= 1;
    if (gameState->player.fuel <= 0) {
        // TODO play a nice animation before doing a game over
        gameOver(gameState);
    }

    // health
    if (gameState->player.health <= 0) {
        // TODO play the explosion before going game over
        gameOver(gameState);
    }

    // shooting
    if (platform.input.shootKeyClicked) {
        glm::vec2 direction = platform.input.mousePosition - gameState->player.position;
        spawnBullet(gameState, gameState->player.position, direction, true);
    }

    Render::Texture *texture = getPlayerTexture(gameState);
    // checking left and right
    if (gameState->player.position.x < texture->width / 2) {
        //		gameState->player.position.x = texture->width / 2;
    } else if (gameState->player.position.x > DEFAULT_WINDOW_WIDTH - texture->width / 2) {
        //		gameState->player.position.x = WINDOW_WIDTH -
        // texture->width / 2;
    }

    // checking top and bottom
    if (gameState->player.position.y < texture->height / 2) {
        //		gameState->player.position.y = texture->height / 2;
    } else if (gameState->player.position.y > DEFAULT_WINDOW_HEIGHT - texture->height / 2) {
        //		gameState->player.position.y = WINDOW_HEIGHT -
        // texture->height / 2;
    }
}

/**
 * Renders the player to the video buffer
 */
void renderPlayer(GameState *gameState) {
    Render::pushTexture(gameState, getPlayerTexture(gameState), gameState->player.position, gameState->player.size,
                        glm::vec2(0, 1), AtomPlane::PLAYER);
}

/**
 * Updates and renders a single bullet
 * Checks for collisions with other cars or the player
 */
bool updateAndRenderBullet(Platform &platform, GameState *gameState, Bullet &bullet, bool isPlayerBullet,
                           bool shouldUpdate) {
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

        Math::Rectangle bulletRect = getBoundingBox(bullet.position, bullet.radius * 2, bullet.radius * 2);

        if (isPlayerBullet) {
            for (int i = 0; i < gameState->world.nextTrafficCarIndex; i++) {
                Car *car = &gameState->world.traffic[i];
                Render::Texture trafficTexture = gameState->resources.trafficCarTextures[car->carIndex];

                // decreases the size of the collision box
                int bufferZone = 17;
                Math::Rectangle trafficRect = getBoundingBox(car->position, trafficTexture.width - bufferZone,
                                                             trafficTexture.height - bufferZone);

                Math::Rectangle collisionBox = getCollisionBox(trafficRect, bulletRect);

#if COLLISION_DEBUG
                Render::pushRectangle(buffer, collisionBox, 0x40ff0000);
#endif

                if (Collision::rectangle(collisionBox, bullet.position)) {
                    // FIXME balance damage
                    car->health -= 1;
                    return true;
                }
            }
        } else {
            Render::Texture *playerTexture = getPlayerTexture(gameState);
            Math::Rectangle playerRect =
                getBoundingBox(gameState->player.position, playerTexture->width, playerTexture->height);
            Math::Rectangle collisionBox = getCollisionBox(playerRect, bulletRect);

#if COLLISION_DEBUG
            Render::pushRectangle(buffer, collisionBox, 0x40ff0000);
#endif

            if (Collision::rectangle(collisionBox, bullet.position)) {
                // FIXME balance damage
                //				gameState->player.health -= 1;
                return true;
            }
        }
    }

    AtomPlane plane = AtomPlane::AI_BULLETS;
    if (isPlayerBullet) {
        plane = AtomPlane::PLAYER_BULLETS;
    }

    Render::pushCircle(gameState, bullet.position, bullet.radius, bullet.color, plane);
    return false;
}

/**
 * Updates and renders all bullets (player and AI)
 */
void updateAndRenderBullets(Platform &platform, GameState *gameState, bool shouldUpdate) {
    for (int i = 0; i < gameState->nextAIBulletIndex; i++) {
        if (updateAndRenderBullet(platform, gameState, gameState->aiBullets[i], false, shouldUpdate)) {
            removeElement(gameState->aiBullets, &gameState->nextAIBulletIndex, &i);
        }
    }

    for (int i = 0; i < gameState->nextPlayerBulletIndex; i++) {
        if (updateAndRenderBullet(platform, gameState, gameState->playerBullets[i], true, shouldUpdate)) {
            removeElement(gameState->playerBullets, &gameState->nextPlayerBulletIndex, &i);
        }
    }
}

/**
 * Spawns a new car on one of the four lanes
 */
void spawnTrafficCar(GameState *gameState) {
    Car car = {};
    car.carIndex = std::rand() % NUM_TRAFFIC_TEXTURES;
    float x = (std::rand() % 4) * (DEFAULT_WINDOW_WIDTH / 4) + DEFAULT_WINDOW_WIDTH / 8;
    car.position = {x, -80};
    car.speed = 5;
    car.health = 75;

    unsigned arrSize = sizeof(gameState->world.traffic) / sizeof(Car);
    if (gameState->world.nextTrafficCarIndex < (int)arrSize) {
        gameState->world.traffic[gameState->world.nextTrafficCarIndex] = car;
        gameState->world.nextTrafficCarIndex++;
    }
}

/**
 * Updates and renders all cars
 * Checks for collision between one of the cars and the player
 */
void updateAndRenderTraffic(VideoBuffer *buffer, GameState *gameState, bool shouldUpdate) {
    if (shouldUpdate && gameState->frameCounter % gameState->trafficFrequency == 0) {
        spawnTrafficCar(gameState);
    }

    if (shouldUpdate && gameState->frameCounter % gameState->bulletFrequency == 0 &&
        gameState->world.nextTrafficCarIndex > 0) {
        // FIXME maybe choose the car that is furthest away from the player
        int carIndex = 0;
        if (gameState->world.nextTrafficCarIndex > 1) {
            carIndex = std::rand() % gameState->world.nextTrafficCarIndex;
        }
        auto &car = gameState->world.traffic[carIndex];
        shootAtPlayer(gameState, car.position);
    }

    for (int i = 0; i < gameState->world.nextTrafficCarIndex; i++) {
        Car *car = &gameState->world.traffic[i];
        Render::Texture *texture = &gameState->resources.trafficCarTextures[car->carIndex];

        if (shouldUpdate) {
            car->position = {car->position.x, ((float)car->speed) + car->position.y};

            if (car->health <= 0) {
                // TODO should we show an explosion?
                removeElement(gameState->world.traffic, &gameState->world.nextTrafficCarIndex, &i);
                continue;
            }

            if (car->position.y - texture->height / 2 > DEFAULT_WINDOW_HEIGHT) {
                removeElement(gameState->world.traffic, &gameState->world.nextTrafficCarIndex, &i);
                continue;
            }

            Math::Rectangle carRect = getBoundingBox(car->position, texture->width, texture->height);

            Render::Texture *playerTexture = getPlayerTexture(gameState);
            Math::Rectangle playerRect =
                getBoundingBox(gameState->player.position, playerTexture->width, playerTexture->height);

            Math::Rectangle collisionBox = getCollisionBox(carRect, playerRect);

#if COLLISION_DEBUG
            Render::pushRectangle(buffer, collisionBox, 0x4000ff00);
#endif

            if (Collision::rectangle(collisionBox, gameState->player.position)) {
                gameOver(gameState);
            }
        }

        //		int x = car->position.x - texture->width / 2;
        //		int y = car->position.y - texture->height / 2;
        //		Render::textureAlpha(buffer, texture, x, y);

        // Render::bar(buffer, { car->position.x, (float) y - 13 }, car->health,
        //		0xff0000ff);
    }
}

/**
 * Spawns a random item on one of the four lanes
 */
void spawnItem(GameState *gameState) {
    Item item = {};
    item.itemIndex = std::rand() % NUM_ITEM_TEXTURES;
    float x = (std::rand() % 4) * (DEFAULT_WINDOW_WIDTH / 4) + DEFAULT_WINDOW_WIDTH / 8;
    item.position = {x, -80};

    unsigned arrSize = sizeof(gameState->world.items) / sizeof(Item);
    if (gameState->world.nextItemIndex < (int)arrSize) {
        gameState->world.items[gameState->world.nextItemIndex] = item;
        gameState->world.nextItemIndex++;
    }
}

/**
 * Updates and renders all items
 * Checks for collision with player and handles pickup effects
 */
void updateAndRenderItems(VideoBuffer *buffer, GameState *gameState, bool shouldUpdate) {
    if (shouldUpdate && gameState->frameCounter % gameState->itemFrequency == 0) {
        spawnItem(gameState);
    }

    for (int i = 0; i < gameState->world.nextItemIndex; i++) {
        Item *item = &gameState->world.items[i];
        Render::Texture *texture = &gameState->resources.itemTextures[item->itemIndex];

        if (shouldUpdate) {
            item->position = {item->position.x, (float)(item->position.y + getRoadSpeed(gameState))};

            // decreases the size of the collision box
            int bufferZone = 10;
            Math::Rectangle rect =
                getBoundingBox(item->position, texture->width - bufferZone, texture->height - bufferZone);

            Render::Texture *playerTexture = getPlayerTexture(gameState);
            Math::Rectangle playerRect =
                getBoundingBox(gameState->player.position, playerTexture->width, playerTexture->height);

            Math::Rectangle collisionRect = getCollisionBox(rect, playerRect);

#if COLLISION_DEBUG
            Render::pushRectangle(buffer, collisionRect, 0x00ffff50);
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
                    gameState->player.fuel += 100;
                    if (gameState->player.fuel > gameState->player.maxFuel) {
                        gameState->player.fuel = gameState->player.maxFuel;
                    }
                    break;
                }
                removeElement(gameState->world.items, &gameState->world.nextItemIndex, &i);
                continue;
            }

            if (item->position.y - texture->height / 2 > DEFAULT_WINDOW_HEIGHT) {
                removeElement(gameState->world.items, &gameState->world.nextItemIndex, &i);
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
void updateAndRenderUI(Platform &platform, bool shouldUpdate) {
    GameState *gameState = getGameState(platform);

    auto energyColor = glm::vec4(0.9, 0.5, 0.1, 1);
    auto healthColor = glm::vec4(0.9, 0.5, 0.1, 1);

    float barWidth = 0.05;
    float screenWidth = 2.0;
    float screenHeight = (1.0 / platform.memory.aspectRatio) * 2.0;

    // energy
    Math::Rectangle fuelBar = {};
    float fuelHeight = gameState->player.fuel / gameState->player.maxFuel * screenHeight;
    fuelBar.position = glm::vec2(-screenWidth / 2.0, -screenHeight / 2.0 + fuelHeight);
    fuelBar.size = glm::vec2(barWidth, fuelHeight);
    Render::pushRectangle(gameState, fuelBar, energyColor, AtomPlane::GAME_UI);

    // health
    Math::Rectangle healthBar = {};
    float healthHeight = gameState->player.health / gameState->player.maxHealth * screenHeight;
    healthBar.position = glm::vec2(-screenWidth / 2.0 + barWidth, -screenHeight / 2.0 + healthHeight);
    healthBar.size = glm::vec2(barWidth, healthHeight);
    Render::pushRectangle(gameState, healthBar, healthColor, AtomPlane::GAME_UI);
}

/**
 * Updates and renders the game with all its entities
 */
void updateAndRenderGame(Platform &platform, GameState *gameState, bool update) {
    if (update) {
        updatePlayer(platform);
    }

    if (gameState->menuState != MenuState::CREDITS) {
        renderPlayer(gameState);
    }

    updateAndRenderRoad(platform, update);

    updateAndRenderBullets(platform, gameState, update);

    if ((gameState->menuState != MenuState::MAIN && gameState->menuState != MenuState::CREDITS)) {
        updateAndRenderUI(platform, update);
    }
}

void game(Platform &platform) {
    GameState *gameState = getGameState(platform);

    updateAndRenderGame(platform, gameState, gameState->menuState == MenuState::GAME);

    if (platform.input.escapeKeyClicked && gameState->menuState == MenuState::GAME) {
        loadMenu(platform, MenuState::PAUSE);
    } else if (gameState->menuState != MenuState::GAME) {
        updateAndRenderMenus(platform);
    }
}
