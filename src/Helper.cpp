void importPixelData(void* src, void* dest, unsigned srcWidth,
		unsigned srcHeight, int offsetX, int offsetY, unsigned destWidth,
		unsigned destHeight) {

	for (unsigned y = 0; y < destHeight; y++) {
		for (unsigned x = 0; x < destWidth; x++) {
			int srcIndex = (srcHeight - (y + offsetY) - 1) * srcWidth + x
					+ offsetX;
			int destIndex = y * destWidth + x;
			uint32_t color = ((uint32_t*) (src))[srcIndex];
			uint8_t red = (color & 0xff000000) >> 24;
			uint8_t green = (color & 0x00ff0000) >> 16;
			uint8_t blue = (color & 0x0000ff00) >> 8;
			uint8_t alpha = color & 0x000000ff;
			color = (alpha << 24) + (blue << 16) + (green << 8) + red;
			((uint32_t*) (dest))[destIndex] = color;
		}
	}
}

Texture readBmpIntoMemory(File file, GameMemory *memory, int offsetX,
		int offsetY, int width, int height, bool output = 1) {
	if (((char*) file.content)[0] != 'B' || (file.content)[1] != 'M') {
		abort(file.name + " is not a bitmap file.");
	}
	int fileHeaderSize = 14;
	BitmapHeader header = *((BitmapHeader*) (file.content + fileHeaderSize));

	if (header.bitsPerPixel != 32) {
		abort("Image must have 32-bit of color depth.");
	}

	if (width == -1 || height == -1) {
		width = header.width;
		height = header.height;
	}

	Texture texture = { };
	texture.width = width;
	texture.height = height;
	texture.bytesPerPixel = header.bitsPerPixel / 8;
	texture.content = reservePermanentMemory(memory,
			texture.width * texture.height * texture.bytesPerPixel);

	importPixelData(file.content + header.size + fileHeaderSize,
			texture.content, header.width, header.height, offsetX, offsetY,
			texture.width, texture.height);
	if (output) {
		printf("Successfully loaded %s.\n", file.name.c_str());
	}
	return texture;
}

Texture readBmpIntoMemory(File file, GameMemory *memory) {
	return readBmpIntoMemory(file, memory, 0, 0, -1, -1);
}

void testInput(Input* input) {
//	printf("(%d|%d)\n", input->mouseX, input->mouseY);
	if (input->upKey) {
		printf("Going up!\n");
	}
	if (input->downKey) {
		printf("Going down!\n");
	}
	if (input->leftKey) {
		printf("Going left!\n");
	}
	if (input->rightKey) {
		printf("Going right!\n");
	}
	if (input->shootKeyPressed) {
		printf("Shooting!\n");
	}
}
