#pragma once

#include "RacingToHell.h"
#include "Resources.h"

GameState *beginFrame(GameMemory *memory, Input *input);
void importPixelData(void *src, void *dest, unsigned srcWidth, unsigned srcHeight, int offsetX, int offsetY,
                     unsigned destWidth, unsigned destHeight);
void checkShaders(GameMemory *memory);
Render::Character *getCharacter(GameState *gameState, char character, unsigned fontSizeIndex);
float calculateTextLength(GameState *gameState, std::string text, Render::FontSize fontSize);
void scaleView(GameState *gameState);
void generateWorld(GameMemory *memory);
Math::Rectangle getCollisionBox(Math::Rectangle rect1, Math::Rectangle rect2);

// FIXME do we really want to use templates here or is there a more performant
// way of doing it?
template <typename T> void removeElement(T arr[], int *lastIndex, int *iteratorIndex) {
    arr[*iteratorIndex] = arr[*lastIndex];
    (*lastIndex)--;
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
void loadFont(GameMemory *memory, std::string fontFileName);
void loadTextureToGraphicsMemory(Render::Texture *texture, void *content);
