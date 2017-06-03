#pragma once
#include <iostream>
#include <vector>
#include <time.h>

#include "platform.h"
#include "Math.h"
#include "Sound.h"

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
	// TODO think about coordinate system for screen
	Math::Vector2f mousePosition;
	bool upKey, downKey, leftKey, rightKey;
	bool pauseKey;
	bool shootKeyPressed;
    bool shootKeyClicked;
};

struct Player {
	Math::Vector2f position;
	uint8_t health, carIndex;
	uint8_t speed;
};

struct TrafficCar {
	Math::Vector2f position;
	uint8_t health, carIndex;
	uint8_t speed;
};

struct Bullet {
	Math::Vector2f position, velocity;
	uint32_t color;
};

struct Resources {
	Texture roadTextures[4];

	Texture playerCarTextures[6];
	Texture trafficCarTextures[7];

	Texture explosion[9 * 9];

    Sound::LoadedSound AIShot;
    Sound::LoadedSound playerShot;
};

struct SoundOutputBuffer {
	int16_t *samples;
	int sampleCount;
	int samplesPerSecond;
};

struct PlayingSound
{
    float volume[2];
    uint32_t samplesPlayed;

    Sound::LoadedSound loadedSound;
};

struct GameState {
    PlayingSound playingSounds[256];
    int lastPlayingSound = -1;

	uint32_t frameCounter;
	uint32_t roadPosition;
	uint8_t level;
	uint8_t difficulty;
	Player player;

	uint32_t trafficFrequency;
	uint32_t bulletFrequency;
	uint8_t bulletSpeed;

	int32_t lastTrafficCarIndex = -1;
	TrafficCar traffic[200];

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

void getSoundSamples(GameMemory *memory, SoundOutputBuffer *soundBUffer);
void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory);

void abort(std::string message);
