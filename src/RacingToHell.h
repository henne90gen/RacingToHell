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
    glm::vec2 position, size;
    int8_t carIndex, lastCarIndex;
    float speed;
    float health, maxHealth;
    float fuel, maxFuel;
};

struct Agent {
    glm::vec2 position, direction, velocity, acceleration, size;
    int8_t carIndex, lastCarIndex;
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
    double animationTimerMs = 0.0;
};

struct Menu {
    MenuItem items[10];
    uint8_t numberMenuItems = 0;
    int8_t currentMenuItemIdx = 0;
    glm::vec2 position = {};
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
    uint16_t nextItemIndex = 0;
    uint16_t nextTrafficCarIndex = 0;

    Item items[200];
    Car traffic[200];
};

enum AtomType {
    RECTANGLE,
    TRIANGLE,
    TEXT,
    TEXTURE,
    CIRCLE,
};

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

    // TODO remove the frame counter and replace all usages with platform.frameTimeMs
    uint32_t frameCounter;

    float roadOffset;

    uint8_t level;
    float levelTime, maxLevelTime;
    int8_t difficulty;
    uint32_t trafficFrequency;
    uint32_t bulletFrequency;
    uint32_t itemFrequency;
    float bulletSpeed;
    bool isInBossFight;

    MenuState menuState = MenuState::MAIN;
    Menu menus[3];
    unsigned menuCount, activeMenuIdx;

    Player player;
    Boss boss;

    Agent agents[1000];
    int agentCount;

    GameWorld world;

    uint16_t nextPlayerBulletIndex = 0;
    Bullet playerBullets[100];

    uint16_t nextAIBulletIndex = 0;
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

GameState *getGameState(Platform *platform);

#ifdef HOT_RELOAD
typedef void update_and_render_func(Platform *platform);
typedef void init_resources_func(Platform *platform);
#else
void update_and_render(Platform *platform);
void init_resources();
#endif

#if SOUND_ENABLE
#define GET_SOUND_SAMPLES(name) void name(GameMemory *memory, SoundBuffer *soundBuffer)
typedef GET_SOUND_SAMPLES(get_sound_samples);
#endif
