#pragma once

namespace font {

struct Character {
	char value;
	unsigned width, height;
	char* bitmap;
};

void renderText(VideoBuffer* buffer, std::string text, int x, int y);

void renderText(VideoBuffer* buffer, std::string text, int x, int y, unsigned fontSize);

void setFontSize(unsigned fontSizeInPixel);

void loadFont(GameMemory* memory, std::string fontFileName);

}
