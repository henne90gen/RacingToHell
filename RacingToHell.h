#pragma once
#include <iostream>
#include "platform.h"

struct VideoBuffer {

	uint32_t width, height, bytesPerPixel;
	void* content;
};

struct Texture {

	uint32_t width, height, bytesPerPixel;
	void* content;
};

struct Input {

	// TODO think about coordinate system for screen
	unsigned mouseX, mouseY;
	bool upKey, downKey, leftKey, rightKey;
	bool pauseKey;
	bool shootKey;
};

struct Player {
	float x, y;
	uint8_t health;
};

struct GameState {
	Player player;
	uint8_t level;
	uint8_t difficulty;

};

void updateAndRender(VideoBuffer *buffer, Input *input, GameMemory *memory);
