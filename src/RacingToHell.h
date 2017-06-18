#pragma once
#include <time.h>

#include "platform.h"
#include "MyMath.h"
#include "Memory.h"
#include "Sound.h"
#include "Renderer.h"
#include "Collision.h"
#include "Font.h"

struct VideoBuffer {

	uint32_t width, height, bytesPerPixel;
	void* content;
};

struct Texture {

	uint32_t width, height;
	uint8_t bytesPerPixel;
	void* content;
};

struct GameFont {
	size_t size;
	void* content;
};

struct Input {
	Math::Vector2f mousePosition;
	// TODO convert these into a bit mask (optimization)
	bool upKey, downKey, leftKey, rightKey;
	bool enterKey, escapeKey;
	bool shootKeyPressed, shootKeyClicked;
};

struct Player {
	Math::Vector2f position;
	uint8_t carIndex;
	int32_t speed, health, maxHealth, energy, maxEnergy;
};

struct Car {
	Math::Vector2f position;
	uint8_t carIndex;
	int8_t health, speed;
};

struct Item {
	Math::Vector2f position;
	uint8_t effect, itemIndex, speed;
};

struct Bullet {
	Math::Vector2f position, velocity;
	uint32_t color, radius;
};

struct SoundOutputBuffer {
	int16_t *samples;
	int sampleCount;
	int samplesPerSecond;
};

struct Resources {
	Texture roadTextures[4];
	Texture playerCarTextures[6];
	Texture trafficCarTextures[7];
	Texture itemTextures[2];

	Texture explosion[9 * 9];

	Sound::LoadedSound AIShot;
	Sound::LoadedSound playerShot;

	Sound::LoadedSound Level1Music;
};

enum MenuState {
	GAME, MAIN, OPTIONS_MAIN, GAME_OVER, PAUSE, OPTIONS_PAUSE, CREDITS
};

struct MenuItem {
	Math::Vector2f position;
	char text[50];
};

struct Menu {
	MenuState state;
	MenuItem items[20];
	uint8_t numberMenuItems;
	int8_t currentMenuItem;
};

struct GameState {
	Sound::PlayingSound playingSounds[256];
	int lastPlayingSound = -1;

	uint32_t frameCounter;
	Menu currentMenu;
	uint32_t roadPosition;
	uint8_t level;
	float levelTime, maxLevelTime;
	uint8_t difficulty;

	Player player;

	uint32_t trafficFrequency;
	uint32_t bulletFrequency;
	uint32_t itemFrequency;
	float bulletSpeed;

	int32_t lastItemIndex = -1;
	Item items[200];

	int32_t lastTrafficCarIndex = -1;
	Car traffic[200];

	int32_t lastPlayerBulletIndex = -1;
	Bullet playerBullets[100];

	int32_t lastAIBulletIndex = -1;
	Bullet aiBullets[1000];

	Resources resources;
};

#pragma pack(push)
struct BitmapHeader {
	uint32_t size; /* Size of this header in bytes */
	uint32_t width; /* Image width in pixels */
	uint32_t height; /* Image height in pixels */
	uint16_t planes; /* Number of color planes */
	uint16_t bitsPerPixel; /* Number of bits per pixel */
	uint32_t compression; /* Compression methods used */
	uint32_t sizeOfBitmap; /* Size of bitmap in bytes */
	uint32_t horzResolution; /* Horizontal resolution in pixels per meter */
	uint32_t vertResolution; /* Vertical resolution in pixels per meter */
	uint32_t colorsUsed; /* Number of colors in the image */
	uint32_t colorsImportant; /* Minimum number of important colors */
};
#pragma pack(pop)

Texture readBmpIntoMemory(File file, GameMemory *memory);

GameState* getGameState(GameMemory* memory);
void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory);

void abort(std::string message);

void updateAndRenderRoad(VideoBuffer *buffer, GameState *gameState);
void renderPlayer(VideoBuffer *buffer, GameState *gameState);
