char *reservePermanentMemory(GameMemory *memory, size_t size) {
	char *Result = memory->permanent + memory->permanentMemoryOffset;

	memory->permanentMemoryOffset += size;

	return Result;
}

void abort(std::string message) {
	fprintf(stderr, message.c_str());
	exit(1);
}

void importPixelData(void* input, void* output, unsigned width,
		unsigned height) {
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			int inputIndex = (height - y - 1) * width + x;
			int outputIndex = y * width + x;
			uint32_t color = ((uint32_t*) (input))[inputIndex];
			uint8_t r = (color & 0xff000000) >> 24;
			uint8_t g = (color & 0x00ff0000) >> 16;
			uint8_t b = (color & 0x0000ff00) >> 8;
			uint8_t a = color & 0x000000ff;
			color = (a << 24) + (r << 16) + (g << 8) + b;
			((uint32_t*) (output))[outputIndex] = color;
		}
	}
}

Texture readBmpIntoMemory(File file, GameMemory *memory) {
	if (((char*) file.content)[0] != 'B' || (file.content)[1] != 'M') {
		abort(file.name + " is not a bitmap file.");
	}
	int fileHeaderSize = 14;
	BitmapHeader header = *((BitmapHeader*) (file.content + fileHeaderSize));

	if (header.bitsPerPixel != 32) {
		abort("Image must have 32-bit of color depth.");
	}

	Texture texture = { };
	texture.x = 0;
	texture.y = 0;
	texture.width = header.width;
	texture.height = header.height;
	texture.bytesPerPixel = header.bitsPerPixel / 8;
	texture.content = reservePermanentMemory(memory,
			texture.width * texture.height * texture.bytesPerPixel);

	importPixelData(file.content + header.size + fileHeaderSize,
			texture.content, texture.width, texture.height);
	printf("Successfully loaded %s.\n", file.name.c_str());
	return texture;
}

void testInput(Input* input) {
	printf("(%d|%d)\n", input->mouseX, input->mouseY);
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
	if (input->shootKey) {
		printf("Shooting!\n");
	}
}

