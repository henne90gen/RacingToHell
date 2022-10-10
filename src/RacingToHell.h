#pragma once

#include <glad/glad.h>

#include "Collision.h"
#include "MyMath.h"
#include "Platform.h"
#include "Renderer.h"
#include "Sound.h"

#define BUFFER_OFFSET(i) ((void *)(i))
#define PLAYER_SPEED 0.01

struct VideoBuffer {
    uint32_t width, height, bytesPerPixel;
    void *content;
};

struct SoundBuffer {
    int16_t *samples;
    int sampleCount;
    int samplesPerSecond;
};

struct Player {
    glm::vec2 position, direction, velocity, acceleration, size;
    int8_t carIndex, nextCarIndex;
    double maxSpeed;
    int32_t health, maxHealth, energy, maxEnergy;
};

struct Car {
    glm::vec2 position;
    uint8_t carIndex;
    int8_t health, speed;
};

struct Item {
    glm::vec2 position;
    uint8_t effect, itemIndex;
};

struct Bullet {
    glm::vec2 position, velocity;
    float radius;
    uint32_t color;
};

struct Resources {
    Render::Texture bulletTexture;
    Render::Texture roadTextures[4];
    Render::Texture playerCarTextures[6];
    Render::Texture trafficCarTextures[7];
    Render::Texture itemTextures[2];
    Render::Texture explosion;
    Render::Texture tank;
    Render::Texture tankCannon;

    char fontName[100];
    Render::Character characterMap[3][100];
    unsigned availableFontSizes[3] = {7, 10, 20};

    Sound::LoadedSound aiShot;
    Sound::LoadedSound playerShot;

    Sound::LoadedSound level1Music;
};

enum class MenuState {
    GAME,
    MAIN,
    GAME_OVER,
    PAUSE,
    CREDITS,
};

enum class MainMenuItem {
    START_GAME = 0,
    CHANGE_CAR = 1,
    CHANGE_DIFFICULTY = 2,
    CREDITS = 3,
    EXIT = 4,
};

enum class PauseMenuItem {
    RESUME,
    MAIN_MENU,
};

enum class GameOverMenuItem {
    SUBMIT_SCORE,
    MAIN_MENU,
};

struct MenuItem {
    char text[50];
    bool bouncy = true;
    double animationTimerMs;
};

struct Menu {
    MenuItem items[10];
    uint8_t numberMenuItems = 0;
    int8_t currentMenuItemIdx = 0;
    glm::vec2 position;
    float lineSpacing = 0.2;
    bool isVisible = true;
};

enum class BossType { TANK, MECH };

struct Boss {
    BossType type;
    glm::vec2 position;
    float speed;
    float health, maxHealth;
    int16_t currentPhase;
    int16_t numberOfPhases;
    int32_t bulletFrequency;
};

struct Tile {
    Math::Rectangle rect;
    glm::vec2 orientation;
    bool traversable;
};

struct GameWorld {
    int32_t lastItemIndex = -1;
    Item items[200];

    int32_t lastTrafficCarIndex = -1;
    Car traffic[200];

    Tile tiles[100];
    unsigned width, height; // in tiles
};

enum AtomType { RECTANGLE, TRIANGLE, TEXT, TEXTURE, CIRCLE, SCALE, NOSCALE };

struct RenderAtom {
    union Content {
        Render::Rectangle rect;
        Render::Text text;
        Render::Circle circle;
        Render::TextureRectangle textureRect;
        Render::Triangle triangle;

        Content() {}
        Content(const Content &other) {}
    };

    Content content;
    AtomType type;
    float plane;
};

struct RenderGroup {
    uint32_t count;
    RenderAtom renderAtoms[1100];
};

struct GameState {
    GLuint glProgram;
    float rotationAngle;
    float scale;

    Sound::PlayingSound playingSounds[256];
    int lastPlayingSound = -1;

    uint32_t frameCounter;
    uint32_t roadPosition;
    uint8_t level;
    float levelTime, maxLevelTime;
    int8_t difficulty;
    uint32_t trafficFrequency;
    uint32_t bulletFrequency;
    uint32_t itemFrequency;
    float bulletSpeed;
    bool isInBossFight;

    MenuState menuState = MenuState::MAIN;
    Menu menus[2];
    unsigned menuCount, activeMenuIdx;

    Player player;

    Player agents[1000];
    int agentCount;

    Boss boss;

    GameWorld world;

    int32_t lastPlayerBulletIndex = -1;
    Bullet playerBullets[100];

    int32_t lastAIBulletIndex = -1;
    Bullet aiBullets[1000];

    RenderGroup renderGroup;

    Resources resources;
};

#pragma pack(push)
struct BitmapHeader {
    uint32_t size;            /* Size of this header in bytes */
    uint32_t width;           /* Image width in pixels */
    uint32_t height;          /* Image height in pixels */
    uint16_t planes;          /* Number of color planes */
    uint16_t bitsPerPixel;    /* Number of bits per pixel */
    uint32_t compression;     /* Compression methods used */
    uint32_t sizeOfBitmap;    /* Size of bitmap in bytes */
    uint32_t horzResolution;  /* Horizontal resolution in pixels per meter */
    uint32_t vertResolution;  /* Vertical resolution in pixels per meter */
    uint32_t colorsUsed;      /* Number of colors in the image */
    uint32_t colorsImportant; /* Minimum number of important colors */
};
#pragma pack(pop)

GameState *getGameState(Platform &platform);
void resetGameState(GameState *gameState);
void spawnBullet(GameState *gameState, glm::vec2 position, glm::vec2 velocity, bool playerBullet);

struct Platform;
void update_and_render(Platform &platform);

#if SOUND_ENABLE
#define GET_SOUND_SAMPLES(name) void name(GameMemory *memory, SoundBuffer *soundBuffer)
typedef GET_SOUND_SAMPLES(get_sound_samples);
#endif
