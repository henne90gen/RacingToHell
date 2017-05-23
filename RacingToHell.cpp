#include "RacingToHell.h"
#include "platform.h"
#include <math.h>

static int counter = 0;
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

void getRange(void* input, void* output, int startIndex, int endIndex) {
	for (int i = 0; i < endIndex - startIndex; i++) {
		((char*) output)[i] = ((char*) input)[startIndex + i];
	}
}

Texture readBmpFile(File file) {
	if (((char*) file.content)[0] != 'B' || ((char*) file.content)[1] != 'M') {
		fprintf(stderr, "Not a .bmp file.\n");
		exit(1);
	}

	void* reader = malloc(4);
	getRange(file.content, reader, 2, 6);
	int fileSize = *((int*) reader);

	int fileHeaderSize = 14;
	getRange(file.content, reader, fileHeaderSize, 18);
	int bitmapHeaderSize = *((int*) reader);

	getRange(file.content, reader, 18, 22);
	int width = *((int*) reader);

	getRange(file.content, reader, 22, 26);
	int height = *((int*) reader);

	reader = malloc(2);
	getRange(file.content, reader, 26, 28);
	short colorPlanes = *((short*) reader);
	getRange(file.content, reader, 28, 30);
	short bitsPerPixel = *((short*) reader);

	int pixelArraySize = fileSize - (fileHeaderSize + bitmapHeaderSize);
	reader = malloc(pixelArraySize);
	getRange(file.content, reader, fileHeaderSize + bitmapHeaderSize, fileSize);

	Texture texture = { };
	texture.width = width;
	texture.height = height;
	texture.bytesPerPixel = bitsPerPixel / 8;
	texture.content = malloc(pixelArraySize);

	getRange(reader, texture.content, 0, pixelArraySize);
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
