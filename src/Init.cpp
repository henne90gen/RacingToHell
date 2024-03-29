#include "Init.h"

#include "Boss.h"
#include "GameMenu.h"
#include "Helper.h"
#include "Memory.h"
#include "Renderer.h"

/**
 * Load all audio clips that are going to be used into memory
 */
void loadAudioClips(Platform &platform) {
    GameState *gameState = getGameState(platform);
    gameState->resources.aiShot = Sound::loadWAV(platform, "res/sound/shotAI.wav");
    gameState->resources.playerShot = Sound::loadWAV(platform, "res/sound/shotPlayer.wav");
    gameState->resources.level1Music = Sound::loadWAV(platform, "res/sound/music/level1.wav");
}

/**
 * Load a texture from a bmp file and pushes it to the graphics card
 */
Render::Texture loadTexture(Platform &platform, const std::string &resource_name, int xDivision = 1,
                            int yDivision = 1) {
    auto resource_opt = get_resource(platform, resource_name);
    if (!resource_opt.has_value()) {
        platform.abort("Failed to load texture " + resource_name);
    }

    auto resource_content = resource_opt.value()->get_content(platform);
    if (resource_content[0] != 'B' || resource_content[1] != 'M') {
        platform.abort(resource_name + " is not a bitmap file.");
    }

    int fileHeaderSize = 14;
    BitmapHeader header = *((BitmapHeader *)(resource_content.data() + fileHeaderSize));

    if (header.bitsPerPixel != 32) {
        platform.abort("Image must have 32-bit of color depth.");
    }

    Render::Texture texture = {};
    texture.width = header.width;
    texture.height = header.height;
    texture.bytesPerPixel = header.bitsPerPixel / 8;
    texture.xDivision = xDivision;
    texture.yDivision = yDivision;
    void *content = reserveTemporaryMemory(platform, texture.width * texture.height * texture.bytesPerPixel);

    importPixelData((void *)(resource_content.data() + header.size + fileHeaderSize), content, header.width,
                    header.height, 0, 0, texture.width, texture.height);

    loadTextureToGraphicsMemory(&texture, content);

    freeTemporaryMemory(platform);

    return texture;
}

/**
 * Load all textures that are going to be used into memory
 */
void loadTextures(Platform &platform) {
    GameState *gameState = getGameState(platform);

    gameState->resources.bulletTexture = loadTexture(platform, "res/textures/bullet.bmp");

    for (int i = 0; i < 4; i++) {
        std::string fileName = "res/textures/roads/road" + std::to_string(i) + ".bmp";
        gameState->resources.roadTextures[i] = loadTexture(platform, fileName);
    }

    for (unsigned i = 0; i < NUM_PLAYER_TEXTURES; i++) {
        std::string playerSprite = "res/textures/cars/player" + std::to_string(i) + ".bmp";
        gameState->resources.playerCarTextures[i] = loadTexture(platform, playerSprite);
    }

    for (unsigned i = 0; i < NUM_TRAFFIC_TEXTURES; i++) {
        std::string carSprite = "res/textures/cars/traffic" + std::to_string(i) + ".bmp";
        gameState->resources.trafficCarTextures[i] = loadTexture(platform, carSprite);
    }

    gameState->resources.itemTextures[TOOLBOX_ID] = loadTexture(platform, "res/textures/toolbox.bmp");

    gameState->resources.itemTextures[CANISTER_ID] = loadTexture(platform, "res/textures/canister.bmp");

    // Explosion sprite
    gameState->resources.explosion = loadTexture(platform, "res/textures/explosion.bmp", 9, 9);

    // Tank boss
    gameState->resources.tank = loadTexture(platform, "res/textures/bosses/tank.bmp");
    gameState->resources.tankCannon = loadTexture(platform, "res/textures/bosses/tank_cannon.bmp");
}

/**
 * Resets everything in the GameState to their initial values
 */
void resetGameState(GameState *gameState) {
    gameState->player = {};
    gameState->player.position = glm::vec2(0, -1.01F);
    gameState->player.size = glm::vec2(0.1F, 0.2F);
    // TODO balance speed
    gameState->player.speed = PLAYER_SPEED;
    // TODO balance health
    gameState->player.health = 100.0F;
    gameState->player.maxHealth = 100.0F;
    // TODO balance energy
    gameState->player.fuel = 1000.0F;
    gameState->player.maxFuel = 1000.0F;

    gameState->level = 0;
    gameState->levelTime = 0;
    gameState->maxLevelTime = 60;
    gameState->difficulty = 1;
    gameState->roadOffset = 0.0F;
    gameState->frameCounter = 0;

    // TODO balance spawn rates
    gameState->trafficCarFrequencyMs = 1000;
    gameState->trafficCarSpawnTimeMs = 0;
    gameState->bulletFrequencyMs = 500;
    gameState->bulletSpawnTimeMs = 0;
    gameState->itemFrequencyMs = 500;
    gameState->itemSpawnTimeMs = 0;

    gameState->bulletSpeed = 0.001f;

    gameState->nextAIBulletIndex = 0;
    gameState->nextPlayerBulletIndex = 0;

    gameState->world = {};
    gameState->world.nextItemIndex = 0;
    gameState->world.nextTrafficCarIndex = 0;

    gameState->isInBossFight = true;
    loadBoss(gameState);

    gameState->menuState = MenuState::MAIN;
}

/**
 * Initialize the game appropriately
 */
void init(Platform &platform) {
    std::srand(1337);
    platform.memory.isInitialized = true;

    auto gameState = (GameState *)reservePermanentMemory(platform, sizeof(GameState));
    *gameState = {};

    glClearColor(1.0, 0, 1.0, 1.0);

    // setting up OpenGL
    gameState->scale = 1.0f;
    gameState->rotationAngle = 0.0f;

    checkShaders(platform);

    loadFont(platform, "res/font/Arial.ttf");
    loadAudioClips(platform);
    loadTextures(platform);

    Sound::output(gameState, &gameState->resources.level1Music, 0.1f, 0.1f, Sound::PLAY_LOOP);

    resetGameState(gameState);
    loadMenu(platform, MenuState::MAIN);
}
