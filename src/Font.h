#pragma once

#include "RacingToHell.h"

struct VideoBuffer;

#define FontSize14 0
#define FontSize18 1
#define FontSize24 2

namespace Text {

const char minChar = ' ';
const char maxChar = '~';

struct Character {
	char value;
    unsigned width, height, bearingY, advanceX;
    int kerning[maxChar - minChar];
	char* bitmap;
};

void renderCharacter(GameMemory *memory, VideoBuffer *buffer,
		Character *character, int offsetX, int offsetY, uint8_t r, uint8_t g,
		uint8_t b);

/**
 * Renders the given text at position (x|y), which is the bottom left corner of the text.
 * Method will crash if fontSize is not one of the available font sizes.
 */
void renderText(GameMemory *memory, VideoBuffer* buffer, std::string text,
		Math::Vector2f position, unsigned fontSize, uint8_t r, uint8_t g,
		uint8_t b);

int* getAvailableFontSizes(int* amount);

void loadFont(GameMemory* memory, std::string fontFileName);

}
