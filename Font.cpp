#include <ft2build.h>
#include FT_FREETYPE_H
#include "RacingToHell.h"
#include "Font.h"

// FIXME compile version 2.8 of the freetype library for windows:
// https://stackoverflow.com/questions/6207176/compiling-freetype-to-dll-as-opposed-to-static-library

namespace font {

FT_Library fontLibrary;
FT_Face face;

Character characterMap[100];

void setFontSize(unsigned fontSizeInPixel) {
int error = FT_Set_Char_Size(face, 0, fontSizeInPixel * 64, 300, 300);
	if (error) {
		fprintf(stderr, "Couldn't set pixel sizes.");
		exit(1);
	}
}

void loadCharacter(GameMemory* memory, char loadCharacter) {
	int glyphIndex = FT_Get_Char_Index(face, loadCharacter);
	int error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
	if (error) {
		fprintf(stderr, "Couldn't load glyph for '%c'.", loadCharacter);
		exit(1);
	}
	if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error) {
			fprintf(stderr, "Couldn't render character '%c' as bitmap.",
					loadCharacter);
			exit(1);
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
	characterMap[loadCharacter - ' '] = newCharacter;
}

void loadFont(GameMemory* memory, std::string fontFileName) {
	int error;
	error = FT_Init_FreeType(&fontLibrary);
	if (error) {
		fprintf(stderr, "Couldn't initialize font library.");
		exit(1);
	}

	error = FT_New_Face(fontLibrary, "./res/font/arial.ttf", 0, &face);
	if (error) {
		fprintf(stderr, "Couldn't load font %s.", fontFileName.c_str());
		exit(1);
	}

	setFontSize(20);

	for (char currentChar = ' ' + 1; currentChar < '~'; currentChar++) {
		loadCharacter(memory, currentChar);
	}
}

Character getCharacter(char character) {
	return characterMap[character - ' '];
}

void renderText(VideoBuffer* buffer, std::string text, int posX, int posY) {
	Character character;
	for (unsigned i = 0; i < text.size(); i++) {
		character = getCharacter(text[i]);

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
						(character.bitmap)[glyphIndex];
			}
		}
		posX += character.width + 10;
	}
}

void renderText(VideoBuffer* buffer, std::string text, int posX, int posY,
		unsigned fontSize) {
	setFontSize(fontSize);
	renderText(buffer, text, posX, posY);
}

}
