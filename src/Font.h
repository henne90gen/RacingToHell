#pragma once

#include "RacingToHell.h"

#define FontSize14 0
#define FontSize18 1
#define FontSize24 2

namespace Text {

struct Character {
	char value;
	unsigned width, height;
	char* bitmap;
};

/**
 * Renders the given text at position (x|y), which is the bottom left corner of the text.
 * Method will crash if fontSize is not one of the available font sizes.
 */
void renderText(VideoBuffer* buffer, std::string text, int x, int y, unsigned fontSize);

void renderCharacterAlpha(VideoBuffer *buffer, char c, int offsetX,
    int offsetY, uint8_t r, uint8_t g, uint8_t b, unsigned fontSize);

int* getAvailableFontSizes(int* amount);

void loadFont(GameMemory* memory, std::string fontFileName);

}
