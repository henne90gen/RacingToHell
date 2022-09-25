#include "Init.h"

#include "Boss.h"
#include "GameMenu.h"
#include "Helper.h"
#include "Renderer.h"

/**
 * Load all audio clips that are going to be used into memory
 */
void loadAudioClips(GameMemory *memory) {
    GameState *gameState = getGameState(memory);
    gameState->resources.aiShot =
        Sound::loadWAV(memory, "../../res/sound/shotAI.wav");
    gameState->resources.playerShot =
        Sound::loadWAV(memory, "../../res/sound/shotPlayer.wav");
    gameState->resources.level1Music =
        Sound::loadWAV(memory, "../../res/sound/music/level1.wav");
}

/**
 * Load a texture from a bmp file and pushes it to the graphics card
 */
Render::Texture loadTexture(GameMemory *memory, std::string fileName,
                            int xDivision = 1, int yDivision = 1) {
    File file = memory->readFile(fileName);
    if (((char *)file.content)[0] != 'B' || (file.content)[1] != 'M') {
        memory->abort(file.name + " is not a bitmap file.");
    }
    int fileHeaderSize = 14;
    BitmapHeader header = *((BitmapHeader *)(file.content + fileHeaderSize));

    if (header.bitsPerPixel != 32) {
        memory->abort("Image must have 32-bit of color depth.");
    }

    Render::Texture texture = {};
    texture.width = header.width;
    texture.height = header.height;
    texture.bytesPerPixel = header.bitsPerPixel / 8;
    texture.xDivision = xDivision;
    texture.yDivision = yDivision;
    void *content = reserveTemporaryMemory(
        memory, texture.width * texture.height * texture.bytesPerPixel);

    importPixelData(file.content + header.size + fileHeaderSize, content,
                    header.width, header.height, 0, 0, texture.width,
                    texture.height);

    memory->freeFile(&file);

    loadTextureToGraphicsMemory(&texture, content);

    freeTemporaryMemory(memory);

    return texture;
}

/**
 * Load all textures that are going to be used into memory
 */
void loadTextures(GameMemory *memory) {
    GameState *gameState = getGameState(memory);

    gameState->resources.bulletTexture =
        loadTexture(memory, "../../res/textures/bullet.bmp");

    for (int i = 0; i < 4; i++) {
        std::string fileName =
            "../../res/textures/roads/road" + std::to_string(i) + ".bmp";
        gameState->resources.roadTextures[i] = loadTexture(memory, fileName);
    }

    for (unsigned i = 0; i < NUM_PLAYER_TEXTURES; i++) {
        std::string playerSprite =
            "../../res/textures/cars/player" + std::to_string(i) + ".bmp";
        gameState->resources.playerCarTextures[i] =
            loadTexture(memory, playerSprite);
    }

    for (unsigned i = 0; i < NUM_TRAFFIC_TEXTURES; i++) {
        std::string carSprite =
            "../../res/textures/cars/traffic" + std::to_string(i) + ".bmp";
        gameState->resources.trafficCarTextures[i] =
            loadTexture(memory, carSprite);
    }

    gameState->resources.itemTextures[TOOLBOX_ID] =
        loadTexture(memory, "../../res/textures/toolbox.bmp");

    gameState->resources.itemTextures[CANISTER_ID] =
        loadTexture(memory, "../../res/textures/canister.bmp");

    // Explosion sprite
    gameState->resources.explosion =
        loadTexture(memory, "../../res/textures/explosion.bmp", 9, 9);

    // Tank boss
    gameState->resources.tank =
        loadTexture(memory, "../../res/textures/bosses/tank.bmp");
    gameState->resources.tankCannon =
        loadTexture(memory, "../../res/textures/bosses/tank_cannon.bmp");
}

/**
 * Resets everything in the GameState to their initial values
 */
void resetGameState(GameState *gameState) {
    gameState->player = {};
    gameState->player.position = glm::vec2(0, 0);
    gameState->player.size = glm::vec2(0.05f, 0.10f);
    //	gameState->player.direction = glm::vec2(1, 0);
    // TODO balance speed
    //	gameState->player.speed = 0;
    gameState->player.maxSpeed = PLAYER_SPEED;
    // TODO balance health
    gameState->player.health = 10;
    gameState->player.maxHealth = 100;
    // TODO balance energy
    gameState->player.energy = 1000;
    gameState->player.maxEnergy = 1000;

    gameState->level = 0;
    gameState->levelTime = 0;
    gameState->maxLevelTime = 60;
    gameState->difficulty = 1;
    gameState->roadPosition = 0;
    gameState->frameCounter = 0;

    // TODO balance spawn rates
    gameState->trafficFrequency = 50;
    gameState->bulletFrequency = 50;
    gameState->itemFrequency = 50;
    gameState->bulletSpeed = 0.005f;

    gameState->lastAIBulletIndex = -1;
    gameState->lastPlayerBulletIndex = -1;

    gameState->world = {};
    gameState->world.lastItemIndex = -1;
    gameState->world.lastTrafficCarIndex = -1;
    gameState->world.width = 10;
    gameState->world.height = 10;

    gameState->isInBossFight = true;
    loadBoss(gameState);

    gameState->menuState =
        MenuState::MAIN; // triggers world generation when loading the game
                         // directly into game mode
}

/**
 * Initialize the game appropriately
 */
void init(GameMemory *memory) {
    std::srand(1337);
    memory->isInitialized = true;

    GameState *gameState =
        (GameState *)reservePermanentMemory(memory, sizeof(GameState));

    *gameState = {};

    char vertexShaderFileName[] = "../../res/shaders/vertex.glsl";
    for (unsigned i = 0; i < sizeof(vertexShaderFileName) / sizeof(char); i++) {
        memory->shaderFileNames[0][i] = vertexShaderFileName[i];
    }
    char fragmentShaderFileName[] = "../../res/shaders/fragment.glsl";
    for (unsigned i = 0; i < sizeof(fragmentShaderFileName) / sizeof(char);
         i++) {
        memory->shaderFileNames[1][i] = fragmentShaderFileName[i];
    }

    glClearColor(1.0, 0, 1.0, 1.0);

    // setting up OpenGL
    gameState->scale = 1.0f;
    gameState->rotationAngle = 0.0f;
    initOpenGL(memory);

    loadFont(memory, "../../res/font/Arial.ttf");

    loadAudioClips(memory);

    loadTextures(memory);

    Sound::output(gameState, &gameState->resources.level1Music, 0.1f, 0.1f,
                  Sound::PLAY_LOOP);

    resetGameState(gameState);
    loadMenu(memory, MenuState::MAIN);
}
