#include "RacingToHell.h"
#include "platform.h"
#include <math.h>

static bool loaded = false;

static Texture texture;

void testGraphics(VideoBuffer *buffer, int color) {
	for (unsigned int y = 0; y < buffer->height; y++) {
		for (unsigned int x = 0; x < buffer->width; x++) {
			((int*) buffer->content)[y * buffer->width + x] = color;
		}
	}
}

void renderTexture(VideoBuffer *buffer, Texture* texture) {
	int pos_x = 0;
	int pos_y = 0;

	for (unsigned y = 0; y < texture->height; y++) {
		for (unsigned x = 0; x < texture->width; x++) {
			int bufferIndex = buffer->width * (pos_y + y) + pos_x + x;
			int textureIndex = y * texture->width + x;
			((int*) buffer->content)[bufferIndex] =
					((int*) texture->content)[textureIndex];
		}
	}
}

void importPixelData(void* input, void* output, unsigned width,
		unsigned height) {
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			int inputIndex = (height - y) * width + x;
			int outputIndex = y * width + x;
			uint32_t color = ((uint32_t*) input)[inputIndex];
			uint8_t r = (color & 0xff000000) >> 24;
			uint8_t g = (color & 0x00ff0000) >> 16;
			uint8_t b = (color & 0x0000ff00) >> 8;
			uint8_t a = color & 0x000000ff;
			((uint32_t*) output)[outputIndex] = (a << 24) + (r << 16) + (g << 8)
					+ b;
		}
	}
}

Texture readBmpFile(File file) {
	if (((char*) file.content)[0] != 'B' || ((char*) file.content)[1] != 'M') {
		fprintf(stderr, "Not a .bmp file.\n");
		exit(1);
	}
	int fileHeaderSize = 14;
	BitmapHeader header = *((BitmapHeader*) ((char*) file.content
			+ fileHeaderSize));

	if (header.bitsPerPixel != 32) {
		fprintf(stderr, "Image must have 32-bit colors.");
		exit(1);
	}

	Texture texture = { };
	texture.width = header.width;
	texture.height = header.height;
	texture.bytesPerPixel = header.bitsPerPixel / 8;
	texture.content = malloc(header.sizeOfBitmap);

	importPixelData(((char*) file.content) + header.size + fileHeaderSize,
			texture.content, header.width, header.height);

	printf("Successfully loaded texture.\n");
	return texture;
}

void updateAndRender(VideoBuffer *buffer, Input *input) {
	if (!loaded) {
		loaded = true;
		char fileName[] = "sample_32bit.bmp";
		texture = readBmpFile(readFile(fileName));
	}
	renderTexture(buffer, &texture);
//	testGraphics(buffer, ((int)(input->upKey) * 255) + (((int)(input->downKey) * 255) << 8) + (((int)(input->shootKey) * 255) << 16));
}
