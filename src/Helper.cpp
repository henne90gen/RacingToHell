// FIXME do we really want to use templates here or is there a more performant way of doing it?
template<typename T> void removeElement(T arr[], int *lastIndex,
		int *iteratorIndex) {
	arr[*iteratorIndex] = arr[*lastIndex];
	(*lastIndex)--;
	(*iteratorIndex)--;
}

/**
 * position - center of the model in question
 * texture - used for width and height of the rectangle
 */
Math::Rectangle getBoundingBox(Math::Vector2f position, unsigned width,
		unsigned height) {
	Math::Rectangle rect = { };
	rect.width = width;
	rect.height = height;
	rect.position.x = position.x - rect.width / 2;
	rect.position.y = position.y - rect.height / 2;
	return rect;
}

/**
 * This adds rect2 to rect1
 * Use the result and the position of rect2 for collision detection
 */
Math::Rectangle getCollisionBox(Math::Rectangle rect1, Math::Rectangle rect2) {
	rect1.width += rect2.width;
	rect1.height += rect2.height;
	rect1.position.x -= rect2.width / 2;
	rect1.position.y -= rect2.height / 2;
	return rect1;
}

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

//Render::Texture readBmpIntoMemory(File file, GameMemory *memory, int offsetX,
//		int offsetY, int width, int height) {
//	if (((char*) file.content)[0] != 'B' || (file.content)[1] != 'M') {
//		memory->abort(file.name + " is not a bitmap file.");
//	}
//	int fileHeaderSize = 14;
//	BitmapHeader header = *((BitmapHeader*) (file.content + fileHeaderSize));
//
//	if (header.bitsPerPixel != 32) {
//		memory->abort("Image must have 32-bit of color depth.");
//	}
//
//	if (width == -1 || height == -1) {
//		width = header.width;
//		height = header.height;
//	}
//
//	Render::Texture texture = { };
//	texture.width = width;
//	texture.height = height;
//	texture.bytesPerPixel = header.bitsPerPixel / 8;
//	texture.content = reservePermanentMemory(memory,
//			texture.width * texture.height * texture.bytesPerPixel);
//
//	importPixelData(file.content + header.size + fileHeaderSize,
//			texture.content, header.width, header.height, offsetX, offsetY,
//			texture.width, texture.height);
//	return texture;
//}

//Render::Texture readBmpIntoMemory(File file, GameMemory* memory) {
//	return readBmpIntoMemory(file, memory, 0, 0, -1, -1);
//}

void checkInputForClicks(Input *input) {
	static bool up, down, left, right, enter, escape;
	input->upKeyClicked = input->upKeyPressed && !up;
	up = input->upKeyPressed;

	input->downKeyClicked = input->downKeyPressed && !down;
	down = input->downKeyPressed;

	input->leftKeyClicked = input->leftKeyPressed && !left;
	left = input->leftKeyPressed;

	input->rightKeyClicked = input->rightKeyPressed && !right;
	right = input->rightKeyPressed;

	input->enterKeyClicked = input->enterKeyPressed && !enter;
	enter = input->enterKeyPressed;

	input->escapeKeyClicked = input->escapeKeyPressed && !escape;
	escape = input->escapeKeyPressed;
}

// Do not use this. It's incredibly inefficient!
void renderControls(VideoBuffer* buffer) {
	uint32_t color = 0x800000ff;
	Math::Vector2f point1 = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 * 3 };
	Math::Vector2f point2 = { WINDOW_WIDTH / 2, WINDOW_HEIGHT };
	Math::Vector2f point3 = { WINDOW_WIDTH / 4 * 3, WINDOW_HEIGHT / 8 * 7 };

	Math::Rectangle rect = { };
	rect.position = point1;
	rect.width = WINDOW_WIDTH / 2;
	rect.height = WINDOW_HEIGHT / 4;
	Render::rectangle(buffer, rect, 0x50ffffff);

	float gapHalf = 2;

	// left
	Render::triangle(buffer, color, point1 + Math::Vector2f { 0, gapHalf },
			point2 - Math::Vector2f { 0, gapHalf }, point3 - Math::Vector2f {
					gapHalf, 0 });

	// top
	point1 =
			Math::Vector2f { WINDOW_WIDTH / 2 + gapHalf, WINDOW_HEIGHT / 4 * 3 };
	point2 = Math::Vector2f { WINDOW_WIDTH - gapHalf, WINDOW_HEIGHT / 4 * 3 };
	Render::triangle(buffer, color, point1, point2, point3 - Math::Vector2f { 0,
			gapHalf });

	// right
	point1 = Math::Vector2f { WINDOW_WIDTH, WINDOW_HEIGHT / 4 * 3 + gapHalf };
	point2 = Math::Vector2f { WINDOW_WIDTH, WINDOW_HEIGHT - gapHalf };
	Render::triangle(buffer, color, point1, point2, point3 + Math::Vector2f {
			gapHalf, 0 });

	// bottom
	point1 = Math::Vector2f { WINDOW_WIDTH / 2 + gapHalf, WINDOW_HEIGHT };
	point2 = Math::Vector2f { WINDOW_WIDTH - gapHalf, WINDOW_HEIGHT };
	Render::triangle(buffer, color, point1, point2, point3 + Math::Vector2f { 0,
			gapHalf });
}

GLuint createVertexBufferObject(const GLsizeiptr size, const GLvoid *data,
		const GLenum usage) {
	GLuint vbo_object;
	glGenBuffers(1, &vbo_object);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_object);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo_object;
}

GLuint compileShader(GameMemory *memory, const GLenum type,
		const GLchar *source, const GLint length) {
	GLuint shaderID = glCreateShader(type);
	GLint compileStatus;

	glShaderSource(shaderID, 1, (const GLchar **) &source, &length);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE) {
		int infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			char *shaderErrorMessage = new char[infoLogLength + 1];
			glGetShaderInfoLog(shaderID, infoLogLength, NULL,
					shaderErrorMessage);
			printf("%s\n", shaderErrorMessage);
		}
		std::string message = "Failed to compile shader. "
				+ std::to_string(type) + "\n" + std::string(source);
		memory->abort(message);
	}

	return shaderID;
}

GLuint linkProgram(GameMemory *memory, const GLuint vertex_shader,
		const GLuint fragment_shader) {
	GLuint programID = glCreateProgram();
	GLint linkStatus;

	glAttachShader(programID, vertex_shader);
	glAttachShader(programID, fragment_shader);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE) {
		int infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			char *programErrorMessage = new char[infoLogLength + 1];
			glGetProgramInfoLog(programID, infoLogLength, NULL,
					programErrorMessage);
			printf("%s\n", programErrorMessage);
		}
		std::string message = "Failed to link shader program.\n";
		memory->abort(message);
	}

	return programID;
}

GLuint buildProgram(GameMemory *memory) {
	File vertexShaderFile = memory->readFile("./res/shaders/vertex.glsl");
	GLuint vertexShader = compileShader(memory, GL_VERTEX_SHADER,
			vertexShaderFile.content, std::strlen(vertexShaderFile.content));
	memory->freeFile(&vertexShaderFile);

	File fragmentShaderFile = memory->readFile("./res/shaders/fragment.glsl");
	GLuint fragmentShader = compileShader(memory, GL_FRAGMENT_SHADER,
			fragmentShaderFile.content,
			std::strlen(fragmentShaderFile.content));
	memory->freeFile(&fragmentShaderFile);

	return linkProgram(memory, vertexShader, fragmentShader);
}

void resizeView(GameMemory* memory, float scale) {
	GameState *gameState = getGameState(memory);
	memory->doResize = false;

	float scaleY = 1.0 * scale;
	float scaleX = (1.0 / memory->aspectRatio) * scale;
	GLfloat scaleMatrix[16] = { scaleX, 0, 0, 0, //
			0, scaleY, 0, 0, //
			0, 0, 1.0, 0, //
			0, 0, 0, 1.0 };
	static GLuint scaleMatrixLocation = glGetUniformLocation(
			gameState->glProgram, "u_ScaleMatrix");
	glUniformMatrix4fv(scaleMatrixLocation, 1, GL_FALSE, &scaleMatrix[0]);
}

GameState* beginFrame(GameMemory *memory, Input *input) {
	if (memory->doResize) {
		resizeView(memory, 1.0);
	}

	Render::clearScreen(0);

	checkInputForClicks (input);

	GameState *gameState = getGameState(memory);
	gameState->frameCounter++;

	return gameState;
}
