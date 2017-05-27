#include <ft2build.h>
#include FT_FREETYPE_H
#include "RacingToHell.h"

// FIXME compile version 2.8 of the freetype library for windows:
// https://stackoverflow.com/questions/6207176/compiling-freetype-to-dll-as-opposed-to-static-library

FT_Library fontLibrary;
FT_Face face;

// FIXME maybe use a class instead of a namespace (Singleton?)
namespace font {

void renderText(VideoBuffer* buffer, std::string text, int posX,
		int posY) {
	int error;
	char character;

	for (unsigned i = 0; i < text.size(); i++) {
		character = text[i];
		int glyphIndex = FT_Get_Char_Index(face, character);
		error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
		if (error) {
			fprintf(stderr, "Couldn't load glyph for '%c'.", character);
			exit(1);
		}
		if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
			error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
			if (error) {
				fprintf(stderr, "Couldn't render character '%c' as bitmap.",
						character);
				exit(1);
			}
		}

		int bytesPerPixel = 4;
		for (int y = face->glyph->bitmap.rows / bytesPerPixel - 1; y >= 0;
				y--) {
			for (unsigned x = 0; x < face->glyph->bitmap.width / bytesPerPixel;
					x++) {
				int bufferIndex = (posY
						- (face->glyph->bitmap.rows / bytesPerPixel - y))
						* buffer->width + (posX + x);
				int glyphIndex = y * face->glyph->bitmap.width + x;
				((uint32_t*) buffer->content)[bufferIndex] |=
						((uint32_t *) face->glyph->bitmap.buffer)[glyphIndex];
			}
		}
		posX += face->glyph->bitmap.width / bytesPerPixel + 10;
	}
}

void setFontSize(unsigned fontSizeInPixel) {
	int error = FT_Set_Pixel_Sizes(face, 0, fontSizeInPixel * 10);
	if (error) {
		fprintf(stderr, "Couldn't set pixel sizes.");
		exit(1);
	}
}

void loadFont(std::string fontFileName) {
	int error;
	error = FT_Init_FreeType(&fontLibrary);
	if (error) {
		fprintf(stderr, "Couldn't initialize font library.");
		exit(1);
	}

	error = FT_New_Face(fontLibrary, "./res/font/arial.ttf", 0, &face);
	if (error) {
		fprintf(stderr, "Couldn't load font %s.", fontFileName);
		exit(1);
	}

	setFontSize(20);
}

}
