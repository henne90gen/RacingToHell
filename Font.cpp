#include <ft2build.h>
#include FT_FREETYPE_H
#include "RacingToHell.h"
#include "Font.h"

// FIXME compile version 2.8 of the freetype library for windows:
// https://stackoverflow.com/questions/6207176/compiling-freetype-to-dll-as-opposed-to-static-library

namespace font {

FT_Library fontLibrary;
FT_Face face;

Character characterMap[3][100];
unsigned availableFontSizes[] = { 5, 10, 20 };

void loadCharacter(GameMemory* memory, char loadCharacter, int fontSize) {
	int glyphIndex = FT_Get_Char_Index(face, loadCharacter);
	int error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
	if (error) {
		abort("Couldn't load glyph for " + std::to_string(loadCharacter));
	}
	if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error) {
			abort(
					"Couldn't render character " + std::to_string(loadCharacter)
							+ " as bitmap.");
		}
	}

	Character newCharacter = { };
	newCharacter.value = loadCharacter;
	newCharacter.width = face->glyph->bitmap.width;
	newCharacter.height = face->glyph->bitmap.rows;
	newCharacter.bitmap = memory->permanent + memory->permanentMemoryOffset;

	unsigned bitmapSizeInPixel = newCharacter.width * newCharacter.height;
	memory->permanentMemoryOffset += bitmapSizeInPixel;

	memcpy(newCharacter.bitmap, face->glyph->bitmap.buffer, bitmapSizeInPixel);
	characterMap[fontSize][loadCharacter - ' '] = newCharacter;
}

int getFontSize(int fontSizeIndex) {
	return availableFontSizes[fontSizeIndex];
}

void loadFont(GameMemory* memory, std::string fontFileName) {
	int error;
	error = FT_Init_FreeType(&fontLibrary);
	if (error) {
		abort("Couldn't initialize font library.");
	}

	error = FT_New_Face(fontLibrary, "./res/font/arial.ttf", 0, &face);
	if (error) {
		abort("Couldn't load font " + fontFileName + ".");
	}

	for (unsigned fontSizeIndex = 0;
			fontSizeIndex < sizeof(availableFontSizes) / 4; fontSizeIndex++) {
		int fontSize = getFontSize(fontSizeIndex);
		int error = FT_Set_Char_Size(face, 0, fontSize * 64, WINDOW_WIDTH, WINDOW_HEIGHT);
		if (error) {
			std::string message = "Couldn't set pixel size to "
					+ std::to_string(fontSize) + ".";
			abort(message);
		}


		for (char currentChar = ' '; currentChar < '~'; currentChar++) {
			loadCharacter(memory, currentChar, fontSizeIndex);
		}
	}
}

Character getCharacter(char character, int fontSizeIndex) {
	return characterMap[fontSizeIndex][character - ' '];
}

int getFontSizeIndex(unsigned fontSize) {
	for (unsigned i = 0; i < sizeof(availableFontSizes) / 4; i++) {
		if (availableFontSizes[i] == fontSize) {
			return i;
		}
	}
	abort("Font size " + std::to_string(fontSize) + " not available.");
	return -1;
}

void renderText(VideoBuffer* buffer, std::string text, int posX, int posY,
		unsigned fontSize) {
	Character character;
	int fontSizeIndex = getFontSizeIndex(fontSize);
	for (unsigned i = 0; i < text.size(); i++) {
		character = getCharacter(text[i], fontSizeIndex);

		for (int y = character.height - 1; y >= 0; y--) {
			int yIndex = posY - (character.height - y);
			if (yIndex >= (int) buffer->height || yIndex < 0) {
				continue;
			}

			for (unsigned x = 0; x < character.width; x++) {
				int xIndex = posX + x;
				if (xIndex >= (int) buffer->width || xIndex < 0) {
					continue;
				}

				int bufferIndex = yIndex * buffer->width + (xIndex);
				int glyphIndex = y * character.width + x;
				((uint32_t*) buffer->content)[bufferIndex] |=
						character.bitmap[glyphIndex];
			}
		}
		posX += character.width + 10;
	}
}

}
