#pragma once

#include "RacingToHell.h"

#define FontSize14 0
#define FontSize18 1
#define FontSize24 2

namespace Font {

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

int* getAvailableFontSizes(int* amount);

void loadFont(GameMemory* memory, std::string fontFileName);

}
