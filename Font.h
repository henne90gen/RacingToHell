#pragma once

namespace font {

void renderText(VideoBuffer* buffer, std::string text, int x, int y);

void setFontSize(unsigned fontSizeInPixel);

void loadFont(std::string fontFileName);

}
