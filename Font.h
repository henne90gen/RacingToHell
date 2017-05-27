#pragma once

namespace font {

void renderText(VideoBuffer* buffer, std::string text, int x, int y);

void renderText(VideoBuffer* buffer, std::string text, int x, int y, unsigned fontSize);

void setFontSize(unsigned fontSizeInPixel);

void loadFont(std::string fontFileName);

}
