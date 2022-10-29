#include "GameDemo.h"

#include "Boss.h"
#include "GameMenu.h"
#include "Helper.h"
#include "Player.h"
#include "Sound.h"

#include <array>

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
 * Updates and renders the road in the background
 */
void updateAndRenderRoad(Platform &platform, GameState *gameState, bool shouldUpdate) {
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
void spawnBullet(GameState *gameState, glm::vec2 position, glm::vec2 direction) {
    direction = glm::normalize(direction);
    // FIXME balance bullet speed
    direction = direction * gameState->bulletSpeed;

    Bullet bullet = {};
    bullet.position = position;
    bullet.velocity = direction;
    // FIXME balance bullet radius
    bullet.radius = 0.05;

    int arrSize = sizeof(gameState->aiBullets) / sizeof(Bullet);
    if (gameState->nextAIBulletIndex < arrSize) {
        bullet.color = glm::vec4(1, 0, 1, 1);
        gameState->aiBullets[gameState->nextAIBulletIndex] = bullet;
        gameState->nextAIBulletIndex++;
    }

    Sound::output(gameState, &gameState->resources.aiShot, 0.1f, 0.1f, Sound::PLAY_ONCE);
}

/**
 * Updates and renders a single bullet
 * Checks for collisions with other cars or the player
 */
bool updateAndRenderBullet(Platform &platform, GameState *gameState, Bullet &bullet, bool isPlayerBullet,
                           bool shouldUpdate) {
    if (shouldUpdate) {
        bullet.position = bullet.position + bullet.velocity * float(platform.frameTimeMs);

        if (bullet.position.x < -1.1 || bullet.position.x > 1.1) {
            return true;
        }
        if (bullet.position.y < -(1.0F / platform.memory.aspectRatio) - 0.1 ||
            bullet.position.y > (1.0F / platform.memory.aspectRatio) + 0.1) {
            return true;
        }

        Math::Rectangle bulletRect = {};
        auto r = bullet.radius / 2.0;
        bulletRect.position = bullet.position - glm::vec2(r, -r);
        bulletRect.size = {bullet.radius, bullet.radius};

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
            auto playerRect = getPlayerCollisionBox(gameState);
            auto collisionBox = getCollisionBox(playerRect, bulletRect);

#define COLLISION_DEBUG 0
#if COLLISION_DEBUG
            Render::pushRectangle(gameState, playerRect, glm::vec4(0, 0, 0, 0.5), AtomPlane::AI_BULLETS - 1);
            Render::pushRectangle(gameState, bulletRect, glm::vec4(0, 0, 1, 1), AtomPlane::AI_BULLETS - 1);
            Render::pushRectangle(gameState, collisionBox, glm::vec4(1, 0, 0, 0.5), AtomPlane::AI_BULLETS - 1);
#endif
#undef COLLISION_DEBUG

            if (Collision::rectangle(collisionBox, bullet.position)) {
                // FIXME balance damage
                gameState->player.health -= 1;
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
void spawnTrafficCar(Platform &platform, GameState *gameState, bool shouldUpdate) {
    if (!shouldUpdate) {
        return;
    }

    gameState->trafficCarSpawnTimeMs += platform.frameTimeMs;
    if (gameState->trafficCarSpawnTimeMs <= gameState->trafficCarFrequencyMs) {
        return;
    }
    gameState->trafficCarSpawnTimeMs = 0;

    unsigned arrSize = sizeof(gameState->world.traffic) / sizeof(Car);
    if (gameState->world.nextTrafficCarIndex >= (int)arrSize) {
        return;
    }

    std::array<float, 4> lanes = {-0.75, -0.25, 0.25, 0.75};
    auto lane = std::rand() % 4;
    auto x = lanes[lane];

    Car car = {};
    car.carIndex = std::rand() % NUM_TRAFFIC_TEXTURES;
    car.size = {0.1, 0.2};
    car.position = {x, 1.0F / platform.memory.aspectRatio + car.size.y};
    car.speed = 0.0003F;
    car.health = 1.0F;

    gameState->world.traffic[gameState->world.nextTrafficCarIndex] = car;
    gameState->world.nextTrafficCarIndex++;
}

void spawnTrafficCarBullet(Platform &platform, GameState *gameState, bool shouldUpdate) {
    if (!shouldUpdate) {
        return;
    }

    if (gameState->world.nextTrafficCarIndex <= 0) {
        return;
    }

    gameState->bulletSpawnTimeMs += platform.frameTimeMs;
    if (gameState->bulletSpawnTimeMs <= gameState->bulletFrequencyMs) {
        return;
    }

    int carIndex = std::rand() % gameState->world.nextTrafficCarIndex;
    auto &car = gameState->world.traffic[carIndex];
    shootAtPlayer(gameState, car.position);
    gameState->bulletSpawnTimeMs = 0;
}

bool updateAndRenderTrafficCar(Platform &platform, GameState *gameState, bool shouldUpdate, Car *car) {
    Render::Texture *texture = &gameState->resources.trafficCarTextures[car->carIndex];

    if (shouldUpdate) {
        car->position.y -= car->speed * platform.frameTimeMs;

        if (car->health <= 0) {
            // TODO show an explosion
            return true;
        }

        if (car->position.y + car->size.y / 2.0 < -1.0F / platform.memory.aspectRatio) {
            return true;
        }

        auto carRect = Math::Rectangle();
        auto offset = car->size / 2.0F;
        offset.y *= -1;
        carRect.position = car->position - offset;
        carRect.size = car->size;
        auto playerRect = getPlayerCollisionBox(gameState);
        auto collisionBox = getCollisionBox(carRect, playerRect);

#define COLLISION_DEBUG 0
#if COLLISION_DEBUG
        Render::pushRectangle(gameState, carRect, glm::vec4(1, 1, 0, 0.5), AtomPlane::AI + 1);
        Render::pushRectangle(gameState, collisionBox, glm::vec4(1, 0, 0, 0.5), AtomPlane::AI + 1);
#endif
#undef COLLISION_DEBUG

        if (Collision::rectangle(collisionBox, gameState->player.position)) {
            gameOver(platform);
        }
    }

    Render::pushTexture(gameState, texture, car->position, car->size, glm::vec2(0, -1), 0, AtomPlane::AI);

    return false;
}
/**
 * Updates and renders all cars
 * Checks for collision between each of the cars and the player
 */
void updateAndRenderTraffic(Platform &platform, GameState *gameState, bool shouldUpdate) {
    spawnTrafficCar(platform, gameState, shouldUpdate);
    spawnTrafficCarBullet(platform, gameState, shouldUpdate);

    for (int i = 0; i < gameState->world.nextTrafficCarIndex; i++) {
        Car *car = &gameState->world.traffic[i];
        if (updateAndRenderTrafficCar(platform, gameState, shouldUpdate, car)) {
            removeElement(gameState->world.traffic, &gameState->world.nextTrafficCarIndex, &i);
        }
    }
}

/**
 * Spawns a random item on one of the four lanes
 */
void spawnItem(Platform &platform, GameState *gameState, bool shouldUpdate) {
    if (!shouldUpdate) {
        return;
    }

    gameState->itemSpawnTimeMs += platform.frameTimeMs;
    if (gameState->itemSpawnTimeMs <= gameState->itemFrequencyMs) {
        return;
    }
    gameState->itemSpawnTimeMs = 0;

    unsigned arrSize = sizeof(gameState->world.items) / sizeof(Item);
    if (gameState->world.nextItemIndex >= (int)arrSize) {
        return;
    }

    Item item = {};
    item.itemIndex = std::rand() % NUM_ITEM_TEXTURES;
    auto &texture = gameState->resources.itemTextures[item.itemIndex];
    item.size = {0.1, 0.1 * (float(texture.height) / float(texture.width))};
    std::array<float, 3> itemLanes = {-0.5, 0.0, 0.5};
    float x = itemLanes[std::rand() % 3];
    item.position = {x, (1.0F / platform.memory.aspectRatio) + item.size.y / 2.0F};

    gameState->world.items[gameState->world.nextItemIndex] = item;
    gameState->world.nextItemIndex++;
}

/**
 * Returns true if the item should be removed.
 */
bool updateAndRenderItem(Platform &platform, GameState *gameState, bool shouldUpdate, Item *item) {
    Render::Texture *texture = &gameState->resources.itemTextures[item->itemIndex];

    if (shouldUpdate) {
        item->position.y -= getRoadSpeed(gameState) * platform.frameTimeMs;

        Math::Rectangle rect = {};
        auto offset = item->size / 2.0F;
        offset.y *= -1;
        rect.position = item->position - offset;
        rect.size = item->size;

        auto playerRect = getPlayerCollisionBox(gameState);
        auto collisionRect = getCollisionBox(rect, playerRect);

#define COLLISION_DEBUG 0
#if COLLISION_DEBUG
        Render::pushRectangle(gameState, collisionRect, glm::vec4(0, 0, 1, 0.5), AtomPlane::AI);
#endif
#undef COLLISION_DEBUG

        if (Collision::rectangle(collisionRect, gameState->player.position)) {
            // TODO balance item effects
            switch (item->itemIndex) {
            case TOOLBOX_ID:
                // TODO test: percentage of maxHealth vs absolute value
                gameState->player.health += 5.0F;
                if (gameState->player.health > gameState->player.maxHealth) {
                    gameState->player.health = gameState->player.maxHealth;
                }
                break;
            case CANISTER_ID:
                // TODO test: percentage of maxEnergy vs absolute value
                gameState->player.fuel += 100.0F;
                if (gameState->player.fuel > gameState->player.maxFuel) {
                    gameState->player.fuel = gameState->player.maxFuel;
                }
                break;
            }
            return true;
        }

        if (item->position.y + (item->size.y / 2.0F) < -1.0F / platform.memory.aspectRatio) {
            return true;
        }
    }

    Render::pushTexture(gameState, texture, item->position, item->size, glm::vec2(0, 1), 0, AtomPlane::AI);

    return false;
}

/**
 * Updates and renders all items
 * Checks for collision with player and handles pickup effects
 */
void updateAndRenderItems(Platform &platform, GameState *gameState, bool shouldUpdate) {
    spawnItem(platform, gameState, shouldUpdate);

    for (int i = 0; i < gameState->world.nextItemIndex; i++) {
        Item *item = &gameState->world.items[i];
        if (updateAndRenderItem(platform, gameState, shouldUpdate, item)) {
            removeElement(gameState->world.items, &gameState->world.nextItemIndex, &i);
        }
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

    updateAndRenderRoad(platform, gameState, update);

    updateAndRenderTraffic(platform, gameState, update);

    updateAndRenderItems(platform, gameState, update);

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
