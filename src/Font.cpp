#include <ft2build.h>
#include FT_FREETYPE_H
#include "Font.h"

namespace Text {

FT_Library fontLibrary;
FT_Face face;

void loadCharacter(GameMemory* memory, char loadCharacter, int fontSize) {
	int currentGlyphIndex = FT_Get_Char_Index(face, loadCharacter);
	int error = FT_Load_Glyph(face, currentGlyphIndex, FT_LOAD_RENDER);
	if (error) {
		memory->abort(
				"Couldn't load glyph for " + std::to_string(loadCharacter));
	}

    bool useKerning = FT_HAS_KERNING(face);;

	Character newCharacter = { };
	newCharacter.value = loadCharacter;
	newCharacter.width = face->glyph->bitmap.width;
	newCharacter.height = face->glyph->bitmap.rows;
	newCharacter.bearingX = face->glyph->bitmap_left;
	newCharacter.bearingY = -face->glyph->bitmap_top;
    newCharacter.advanceX = useKerning ? face->glyph->advance.x >> 6 : 0;

	for (char nextChar = minChar; nextChar < maxChar; nextChar++) {
		int nextGlyphIndex = FT_Get_Char_Index(face, nextChar);

		FT_Vector kerning;
		FT_Get_Kerning(face, currentGlyphIndex, nextGlyphIndex,
				FT_KERNING_DEFAULT, &kerning);

		newCharacter.kerning[nextChar - minChar] = kerning.x >> 6;
	}
	unsigned bitmapSizeInPixel = newCharacter.width * newCharacter.height;

	newCharacter.bitmap = reservePermanentMemory(memory, bitmapSizeInPixel);

	memcpy(newCharacter.bitmap, face->glyph->bitmap.buffer, bitmapSizeInPixel);
	GameState *gameState = getGameState(memory);
	gameState->resources.characterMap[fontSize][loadCharacter - minChar] =
			newCharacter;
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
			fontSizeIndex < sizeof(gameState->resources.availableFontSizes) / 4;
			fontSizeIndex++) {
		int fontSize = getFontSize(gameState, fontSizeIndex);
//		int error = FT_Set_Char_Size(face, 0, fontSize*32, WINDOW_WIDTH, WINDOW_HEIGHT);
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
		unsigned fontSizeIndex) {
	return &gameState->resources.characterMap[fontSizeIndex][character - minChar];
}

/**
 * Renders the given text with the given color to the screen.
 * Position is the bottom left corner of the text.
 * fontSizeID needs to be one of the following: FontSizeSmall, FontSizeMedium or FontSizeBig.
 */
void renderText(GameMemory *memory, VideoBuffer* buffer, std::string text,
		Math::Vector2f position, FontSize fontSize, uint8_t r, uint8_t g,
		uint8_t b) {
	GameState *gameState = getGameState(memory);

	int currentX = position.x;

	for (unsigned characterIndex = 0; characterIndex < text.size();
			++characterIndex) {
		Character *character = getCharacter(gameState, text[characterIndex],
				fontSize);

		renderCharacter(memory, buffer, character,
				currentX + character->bearingX, position.y, r, g, b);

		if (characterIndex < text.size() - 1) {
			currentX += character->advanceX
					+ character->kerning[text[characterIndex + 1] - minChar];
		}
	}
}

void renderCharacter(GameMemory *memory, VideoBuffer *buffer,
		Character *character, int offsetX, int offsetY, uint8_t r, uint8_t g,
		uint8_t b) {
	offsetY += character->bearingY;

	uint32_t characterColor = (r) | (g << 8) | (b << 16);

	uint32_t *currentBufferPixel = (uint32_t *) buffer->content
			+ offsetY * (int) buffer->width + offsetX;
	uint8_t *currentTexturePixel = (uint8_t *) character->bitmap;

	uint32_t nextLine = buffer->width - character->width;

	for (int y = 0; y < (int) character->height; ++y) {
		if (offsetY + y < 0) {
			currentBufferPixel += buffer->width;
			currentTexturePixel += character->width;
			continue;
		} else if (offsetY + y >= (int) buffer->height) {
			break;
		}

		for (int x = 0; x < (int) character->width; ++x) {
			if (offsetX + x < 0 || offsetX + x >= (int) buffer->width) {
				currentBufferPixel++;
				currentTexturePixel++;
				continue;
			}

			Render::blendColor(characterColor | (*currentTexturePixel << 24),
					currentBufferPixel);
			currentBufferPixel++;
			currentTexturePixel++;
		}

		currentBufferPixel += nextLine;
	}
}

}
