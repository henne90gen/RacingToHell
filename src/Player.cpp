#include "Player.h"

#include "GameMenu.h"

/**
 * Spawns a bullet at the location of the player
 */
void spawnPlayerBullet(GameState *gameState, glm::vec2 position, glm::vec2 direction) {
    direction = Math::normalize(direction);
    // FIXME balance bullet speed
    direction = direction * gameState->bulletSpeed;

    Bullet bullet = {};
    bullet.position = position;
    bullet.velocity = direction;
    // FIXME balance bullet radius
    bullet.radius = 0.05;

    int arrSize = sizeof(gameState->aiBullets) / sizeof(Bullet);
    if (gameState->nextPlayerBulletIndex < arrSize) {
        bullet.color = glm::vec4(1, 0, 1, 1);
        gameState->playerBullets[gameState->nextPlayerBulletIndex] = bullet;
        gameState->nextPlayerBulletIndex++;
    }

    Sound::output(gameState, &gameState->resources.playerShot, 0.1f, 0.1f, Sound::PLAY_ONCE);
}

/**
 * Retrieves a pointer to the current player car texture
 */
Render::Texture *getPlayerTexture(GameState *gameState) {
    return &gameState->resources.playerCarTextures[gameState->player.carIndex];
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
        spawnPlayerBullet(gameState, gameState->player.position, direction);
    }

    Render::Texture *texture = getPlayerTexture(gameState);
    // checking left and right
    auto leftBound = -1.0F + gameState->player.size.x / 2.0F;
    auto rightBound = 1.0F - gameState->player.size.x / 2.0F;
    if (gameState->player.position.x < leftBound) {
        gameState->player.position.x = leftBound;
    } else if (gameState->player.position.x > rightBound) {
        gameState->player.position.x = rightBound;
    }

    // checking top and bottom
    auto lowerBound = -(1.0F / platform.memory.aspectRatio) + gameState->player.size.y / 2.0F;
    auto topBound = (1.0F / platform.memory.aspectRatio) - gameState->player.size.y / 2.0F;
    if (gameState->player.position.y < lowerBound) {
        gameState->player.position.y = lowerBound;
    } else if (gameState->player.position.y > topBound) {
        gameState->player.position.y = topBound;
    }
}

/**
 * Renders the player to the video buffer
 */
void renderPlayer(GameState *gameState) {
    Render::pushTexture(gameState, getPlayerTexture(gameState), gameState->player.position, gameState->player.size,
                        glm::vec2(0, 1), AtomPlane::PLAYER);
}

Math::Rectangle getPlayerCollisionBox(GameState *gameState) {
    Math::Rectangle playerRect = {};
    auto size = gameState->player.size;
    size.x *= 0.85;
    playerRect.position = gameState->player.position;
    playerRect.position.x -= size.x / 2.0F;
    playerRect.position.y += size.y / 2.0F;
    playerRect.size = size;
    return playerRect;
}
