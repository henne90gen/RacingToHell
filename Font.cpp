#include <ft2build.h>
#include FT_FREETYPE_H
#include "RacingToHell.h"

// FIXME compile version 2.8 of the freetype library for windows:
// https://stackoverflow.com/questions/6207176/compiling-freetype-to-dll-as-opposed-to-static-library

FT_Library fontLibrary;

void renderText(VideoBuffer* buffer, char character, int x, int y) {
	printf("Printing: %c\n", character);
}

void initFont() {
	int error = FT_Init_FreeType(&fontLibrary);
	if (error) {
		fprintf(stderr, "Couldn't initialize font library.");
		exit(1);
	}
}
