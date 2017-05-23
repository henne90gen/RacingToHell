#include "RacingToHell.h"
#include "platform.h"

static int counter = 0;
static bool loaded = false;
static Texture texture;

void testGraphics(VideoBuffer *buffer) {
	for (unsigned int y = 0; y < buffer->height; y++) {
		for (unsigned int x = 0; x < buffer->width; x++) {
			((int*) buffer->content)[y * buffer->width + x] = counter++;
		}
	}
}

void renderTexture(VideoBuffer *buffer, Texture* texture) {
	for (int y = 0; y < texture->height; y++) {
		for (int x = 0; x < texture->width; x++) {
			int index = y * buffer->width + x;
			((int*) buffer->content)[index] = ((int*) texture->content)[index];
		}
	}
}

void updateAndRender(VideoBuffer *buffer) {
	testGraphics(buffer);
	renderTexture(buffer, &texture);
	if (!loaded) {
		loaded = true;
		char fileName[] = "sample.bmp";
		readImageFile(&texture, fileName);
	}
}
