#include <ft2build.h>
#include FT_FREETYPE_H
#include "Font.h"

namespace Text {

FT_Library fontLibrary;
FT_Face face;

void loadCharacter(GameMemory* memory, char loadCharacter, int fontSize) {
	int currentGlyphIndex = FT_Get_Char_Index(face, loadCharacter);
	int error = FT_Load_Glyph(face, currentGlyphIndex, FT_LOAD_DEFAULT);
	if (error) {
		memory->abort(
				"Couldn't load glyph for " + std::to_string(loadCharacter));
	}

	if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error) {
			memory->abort(
					"Couldn't render character " + std::to_string(loadCharacter)
							+ " as bitmap.");
		}
	}

	Character newCharacter = { };
	newCharacter.value = loadCharacter;
	newCharacter.width = face->glyph->bitmap.width;
	newCharacter.height = face->glyph->bitmap.rows;
    newCharacter.bearingY = -face->glyph->bitmap_top;
    newCharacter.advanceX = face->glyph->advance.x >> 6;

    for (char nextChar = minChar; nextChar < maxChar; nextChar++)
    {
        int nextGlyphIndex = FT_Get_Char_Index(face, nextChar);

        FT_Vector kerning;
        FT_Get_Kerning(face, currentGlyphIndex, nextGlyphIndex, FT_KERNING_DEFAULT, &kerning);

        newCharacter.kerning[nextChar - minChar] = kerning.x >> 6;
    }

	unsigned bitmapSizeInPixel = newCharacter.width * newCharacter.height;

	newCharacter.bitmap = reservePermanentMemory(memory, bitmapSizeInPixel);

	memcpy(newCharacter.bitmap, face->glyph->bitmap.buffer, bitmapSizeInPixel);
	GameState *gameState = getGameState(memory);
	gameState->resources.characterMap[fontSize][loadCharacter - minChar] = newCharacter;
}

int getFontSize(GameState *gameState, int fontSizeIndex) {
	return gameState->resources.availableFontSizes[fontSizeIndex];
}

void loadFont(GameMemory* memory, std::string fontFileName) {
	int error;
	error = FT_Init_FreeType(&fontLibrary);
	if (error) {
		memory->abort("Couldn't initialize font library.");
	}

	File fontFile = memory->readFile(fontFileName);
	error = FT_New_Memory_Face(fontLibrary, (const FT_Byte *) fontFile.content,
			fontFile.size, 0, &face);
	if (error) {
		memory->abort("Couldn't load font " + fontFileName + ".");
	}

	GameState *gameState = getGameState(memory);

	for (unsigned fontSizeIndex = 0;
			fontSizeIndex < sizeof(gameState->resources.availableFontSizes) / 4; fontSizeIndex++) {
		int fontSize = getFontSize(gameState, fontSizeIndex);
//		int error = FT_Set_Char_Size(face, 0, fontSize * 64, WINDOW_WIDTH, WINDOW_HEIGHT);
		int error = FT_Set_Pixel_Sizes(face, fontSize * 2, 0);
		if (error) {
			std::string message = "Couldn't set pixel size to "
					+ std::to_string(fontSize) + ".";
			memory->abort(message);
		}

		for (char currentChar = minChar; currentChar < maxChar; currentChar++) {
			loadCharacter(memory, currentChar, fontSizeIndex);
		}
	}
}

Character *getCharacter(GameState *gameState, char character,
		int fontSizeIndex) {
	return &gameState->resources.characterMap[fontSizeIndex][character - ' '];
}

int getFontSizeIndex(GameMemory *memory, unsigned fontSize) {
	GameState *gameState = getGameState(memory);
	for (unsigned i = 0;
			i < sizeof(gameState->resources.availableFontSizes) / 4; i++) {
		if (gameState->resources.availableFontSizes[i] == fontSize) {
			return i;
		}
	}
	memory->abort("Font size " + std::to_string(fontSize) + " not available.");
	return -1;
}

void renderText(GameMemory *memory, VideoBuffer* buffer, std::string text,
		Math::Vector2f position, unsigned fontSize) {
	GameState *gameState = getGameState(memory);
	int posX = position.x;
	int posY = position.y;
	Character *character;
	int fontSizeIndex = getFontSizeIndex(memory, fontSize);
	for (unsigned i = 0; i < text.size(); i++) {
		character = getCharacter(gameState, text[i], fontSizeIndex);

		for (int y = character->height - 1; y >= 0; y--) {
			int yIndex = posY - (character->height - y);
			if (yIndex >= (int) buffer->height || yIndex < 0) {
				continue;
			}

			for (unsigned x = 0; x < character->width; x++) {
				int xIndex = posX + x;
				if (xIndex >= (int) buffer->width || xIndex < 0) {
					continue;
				}

				int bufferIndex = yIndex * buffer->width + (xIndex);
				int glyphIndex = y * character->width + x;
				((uint32_t*) buffer->content)[bufferIndex] |=
						character->bitmap[glyphIndex];
			}
		}
		posX += character->width + 10;
	}
}

void renderTextColored(GameMemory *memory, VideoBuffer* buffer, std::string text,
    Math::Vector2f position, unsigned fontSize, uint8_t r, uint8_t g, uint8_t b)
{
    GameState *gameState = getGameState(memory);

    int currentX = position.x;
    int fontSizeIndex = getFontSizeIndex(memory, fontSize);

    for (unsigned characterIndex = 0; characterIndex < text.size(); ++characterIndex)
    {
        Character *character = getCharacter(gameState, text[characterIndex], fontSizeIndex);

        renderCharacterAlpha(memory, buffer, character, currentX, position.y, r, g, b);
    
        currentX += character->advanceX + ((characterIndex < text.size() - 1) ? character->kerning[characterIndex + 1] : 0);
    }
}

void renderCharacterAlpha(GameMemory *memory, VideoBuffer *buffer, Character *character,
		int offsetX, int offsetY, uint8_t r, uint8_t g, uint8_t b) {
    offsetY += character->bearingY;
    
    uint32_t characterColor = (r) | (g << 8) | (b << 16);

    uint32_t *currentBufferPixel = (uint32_t *)buffer->content
        + offsetY * (int)buffer->width + offsetX;
    uint8_t *currentTexturePixel = (uint8_t *)character->bitmap;

    uint32_t nextLine = buffer->width - character->width;

    for (int y = 0; y < (int)character->height; ++y) {
        if (offsetY + y < 0) {
            currentBufferPixel += buffer->width;
            currentTexturePixel += character->width;
            continue;
        }
        else if (offsetY + y >= (int)buffer->height) {
            break;
        }

        for (int x = 0; x < (int)character->width; ++x) {
            if (offsetX + x < 0 || offsetX + x >= (int)buffer->width) {
                currentBufferPixel++;
                currentTexturePixel++;
                continue;
            }

            Render::blendColor(characterColor | (*currentTexturePixel << 24), currentBufferPixel);
            currentBufferPixel++;
            currentTexturePixel++;
        }

        currentBufferPixel += nextLine;
    }
}



void renderTextAlpha(VideoBuffer *buffer, Render::Texture* texture, int offsetX,
		int offsetY) {

}

}
