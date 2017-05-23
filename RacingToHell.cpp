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
	for (unsigned y = 0; y < texture->height; y++) {
		for (unsigned x = 0; x < texture->width; x++) {
			int index = y * buffer->width + x;
//			((int*) buffer->content)[index] = ((int*) texture->content)[index];
		}
	}
}

void getRange(char* input, char* output, int startIndex, int endIndex) {
	for (int i = 0; i < endIndex - startIndex; i++) {
		output[i] = input[startIndex + i];
	}
}

void readBmpFile(Texture* texture, char* content, char* fileName) {
	if (content[0] != 'B' || content[1] != 'M') {
		fprintf(stderr, "Not a .bmp file.\n");
		exit(1);
	}

	char* reader = (char*) malloc(4);
	getRange(content, reader, 2, 6);
	int fileSize = *((int*) reader);

	int fileHeaderSize = 14;
	getRange(content, reader, fileHeaderSize, 18);
	int bitmapHeaderSize = *((int*) reader);

	getRange(content, reader, 18, 22);
	int width = *((int*) reader);

	getRange(content, reader, 22, 26);
	int height = *((int*) reader);

	reader = (char*) malloc(2);
	getRange(content, reader, 26, 28);
	short colorPlanes = *((short*) reader);
	getRange(content, reader, 28, 30);
	short bitsPerPixel = *((short*) reader);

	int colorPaletteSize = (1 << bitsPerPixel) * 4;
	long int pixelArraySize = fileSize
			- (fileHeaderSize + bitmapHeaderSize + colorPaletteSize);
	reader = (char*) malloc(pixelArraySize);
	getRange(content, reader, fileHeaderSize + bitmapHeaderSize, fileSize);

	texture->width = width;
	texture->height = height;
	texture->bytesPerPixel = bitsPerPixel;
	texture->content = malloc(width * height * 4);

	int rowSize = floor((bitsPerPixel * width + 31) / 32) * 4;

	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {

		}
	}
//	getRange(reader, (char*) texture->content, 0, pixelArraySize);
	printf("Successfully loaded image %s.\n", fileName);
}

void updateAndRender(VideoBuffer *buffer, Input *input) {
	if (!loaded) {
		loaded = true;
		char fileName[] = "sample.bmp";
		char* content = (char*) malloc(getFileSizePlusOne(fileName));
		readFile(fileName, content);
		readBmpFile(&texture, content, fileName);
	}

	renderTexture(buffer, &texture);
	testGraphics(buffer, ((int)(input->upKey) * 255) + (((int)(input->downKey) * 255) << 8) + (((int)(input->shootKey) * 255) << 16));
}
