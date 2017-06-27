#pragma once

#include "RacingToHell.h"

struct VideoBuffer;

namespace Text {

const char minChar = ' ';
const char maxChar = '~';

enum FontSize {
	Small, Medium, Big
};

struct Character {
	char value;
    unsigned width, height, bearingX, bearingY, advanceX;
    int kerning[maxChar - minChar];
	char* bitmap;
};

void renderCharacter(GameMemory *memory, VideoBuffer *buffer,
		Character *character, int offsetX, int offsetY, uint8_t r, uint8_t g,
		uint8_t b);

void renderText(GameMemory *memory, VideoBuffer* buffer, std::string text,
		Math::Vector2f position, FontSize fontSize, uint8_t r, uint8_t g,
		uint8_t b);

int* getAvailableFontSizes(int* amount);

void loadFont(GameMemory* memory, std::string fontFileName);

}
