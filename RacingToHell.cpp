#include "RacingToHell.h"

static int counter = 0;

void testGraphics(VideoBuffer *buffer) {
	for (unsigned int y = 0; y < buffer->height; y++) {
		for (unsigned int x = 0; x < buffer->width; x++) {
			((int*) buffer->content)[y * buffer->width + x] = counter++;
		}
	}
}

void updateAndRender(VideoBuffer *buffer) {
//	testGraphics(buffer);
}
