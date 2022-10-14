#pragma once

#include "RacingToHell.h"
#include "Resources.h"

GameState *beginFrame(Platform &platform);
void importPixelData(void *src, void *dest, unsigned srcWidth, unsigned srcHeight, int offsetX, int offsetY,
                     unsigned destWidth, unsigned destHeight);
void checkShaders(Platform &platform);
Render::Character *getCharacter(GameState *gameState, char character, unsigned fontSizeIndex);
glm::vec2 calculateTextSize(GameState *gameState, std::string text, Render::FontSize fontSize);
void scaleView(GameState *gameState);
Math::Rectangle getCollisionBox(Math::Rectangle rect1, Math::Rectangle rect2);

template <typename T> void removeElement(T arr[], uint16_t *nextIndex, int *iteratorIndex) {
    (*nextIndex)--;
    arr[*iteratorIndex] = arr[*nextIndex];
    (*iteratorIndex)--;
}

/**
 * position - center of the model in question
 * texture - used for width and height of the rectangle
 */
Math::Rectangle getBoundingBox(glm::vec2 position, unsigned width, unsigned height);

void checkPlayerTileCollision(Player *player, Tile *tile);

int getFontSize(GameState *gameState, int fontSizeIndex);
void extractFileName(std::string fileName, const std::string &fileExtension, char result[]);
void loadFont(Platform &platform, const std::string &fontFileName);
void loadTextureToGraphicsMemory(Render::Texture *texture, void *content);
