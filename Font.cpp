#include <ft2build.h>
#include FT_FREETYPE_H
#include "RacingToHell.h"

// FIXME compile version 2.8 of the freetype library for windows:
// https://stackoverflow.com/questions/6207176/compiling-freetype-to-dll-as-opposed-to-static-library

FT_Library fontLibrary;
FT_Face face;

void renderText(VideoBuffer* buffer, char character, int posX, int posY) {
	int error;
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
	printf("%d\n", face->glyph->bitmap.pitch);
	int bytesPerPixel = 4;
	for (unsigned y = 0; y < face->glyph->bitmap.rows / bytesPerPixel; y++) {
		for (unsigned x = 0; x < face->glyph->bitmap.width / bytesPerPixel;
				x++) {
			int bufferIndex = (posY + y) * buffer->width + (posX + x);
			int glyphIndex = y * face->glyph->bitmap.width + x;
			((uint32_t*) buffer->content)[bufferIndex] =
					((uint32_t *) face->glyph->bitmap.buffer)[glyphIndex];
		}
	}
}

void loadFont(char* fontFileName) {
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

//	error = FT_Set_Char_Size(face, 0, 16 * 64, 72, 72);
//	if (error) {
//		fprintf(stderr, "Couldn't set character size.");
//		exit(1);
//	}

	error = FT_Set_Pixel_Sizes(face, 0, 500);
	if (error) {
		fprintf(stderr, "Couldn't set pixel sizes.");
		exit(1);
	}
}
