#include "RacingToHell.h"

static int counter = 0;
static bool loaded = false;

void testGraphics(VideoBuffer *buffer, int color) {
	for (unsigned int y = 0; y < buffer->height; y++) {
		for (unsigned int x = 0; x < buffer->width; x++) {
			((int*) buffer->content)[y * buffer->width + x] = color;
		}
	}
}

void updateAndRender(VideoBuffer *buffer, Input *input) {
	if (!loaded) {
		loaded = true;
		char fileName[] = "HelloWorld.bmp";
		//readImageFile(buffer->content, fileName);
	}

    testGraphics(buffer, ((int)(input->upKey) * 255) + (((int)(input->downKey) * 255) << 8));
}
