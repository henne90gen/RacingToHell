#include "Platform.h"

// FIXME do we really want to use templates here or is there a more performant way of doing it?
template<typename T> void removeElement(T arr[], int *lastIndex,
		int *iteratorIndex) {
	arr[*iteratorIndex] = arr[*lastIndex];
	(*lastIndex)--;
	(*iteratorIndex)--;
}

Render::Character *getCharacter(GameState *gameState, char character,
		unsigned fontSizeIndex) {
	return &gameState->resources.characterMap[fontSizeIndex][character
			- Render::firstCharacter];
}

int getFontSize(GameState *gameState, int fontSizeIndex) {
	return gameState->resources.availableFontSizes[fontSizeIndex];
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
	rect1.size.x += rect2.size.x;
	rect1.size.y += rect2.size.y;
	rect1.position.x -= rect2.size.x / 2.0f;
	rect1.position.y -= rect2.size.y / 2.0f;
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

void checkInputForClicks(Input *input) {
	static bool up, down, left, right, enter, escape, plus, minus;

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

	input->plusKeyClicked = input->plusKeyPressed && !plus;
	plus = input->plusKeyPressed;

	input->minusKeyClicked = input->minusKeyPressed && !minus;
	minus = input->minusKeyPressed;
}

GLuint createVBO(const GLsizeiptr size, const GLvoid *data,
		const GLenum usage) {
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;
}

GLuint compileShader(GameMemory *memory, const GLenum type,
		std::string shaderFileName) {

	File shaderFile = memory->readFile(shaderFileName);

	GLint compileStatus;
	GLuint shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, (GLchar **) &shaderFile.content,
			(GLint *) &shaderFile.size);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE) {
		int infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			char *shaderErrorMessage = new char[infoLogLength + 1];
			glGetShaderInfoLog(shaderID, infoLogLength, NULL,
					shaderErrorMessage);
			memory->log(shaderErrorMessage);
		}
		std::string message = "Failed to compile shader. "
				+ std::to_string(type) + "\n" + std::string(shaderFile.content);
		memory->abort(message);
	}

	memory->freeFile(&shaderFile);

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
			memory->log(programErrorMessage);
		}
		std::string message = "Failed to link shader program.\n";
		memory->abort(message);
	}

	return programID;
}

GLuint buildProgram(GameMemory *memory) {
	GLuint vertexShader = compileShader(memory, GL_VERTEX_SHADER,
			memory->shaderFileNames[0]);

	GLuint fragmentShader = compileShader(memory, GL_FRAGMENT_SHADER,
			memory->shaderFileNames[1]);

	return linkProgram(memory, vertexShader, fragmentShader);
}

void setScaleToIdentity(GameState *gameState) {
	GLfloat scaleMatrix[16] = { 1.0, 0, 0, 0, //
			0, 1.0, 0, 0, //
			0, 0, 1.0, 0, //
			0, 0, 0, 1.0 };

	GLuint scaleMatrixLocation = glGetUniformLocation(gameState->glProgram,
			"u_ScaleMatrix");
	glUniformMatrix4fv(scaleMatrixLocation, 1, GL_FALSE, &scaleMatrix[0]);
}

void scaleView(GameState *gameState) {
	GLfloat scaleMatrix[16] = { gameState->scale, 0, 0, 0, //
			0, gameState->scale, 0, 0, //
			0, 0, 1.0, 0, //
			0, 0, 0, 1.0 };

	GLuint scaleMatrixLocation = glGetUniformLocation(gameState->glProgram,
			"u_ScaleMatrix");
	glUniformMatrix4fv(scaleMatrixLocation, 1, GL_FALSE, &scaleMatrix[0]);
}

void resizeView(GameMemory* memory) {
	GameState *gameState = getGameState(memory);
	memory->doResize = false;

	GLfloat aspectRatioMatrix[16] = { 1.0f / memory->aspectRatio, 0, 0, 0, //
			0, 1.0, 0, 0, //
			0, 0, 1.0, 0, //
			0, 0, 0, 1.0 };
	GLuint aspectRatioMatrixLocation = glGetUniformLocation(
			gameState->glProgram, "u_AspectRatioMatrix");
	glUniformMatrix4fv(aspectRatioMatrixLocation, 1, GL_FALSE,
			&aspectRatioMatrix[0]);
}

void initOpenGL(GameMemory* memory) {
	GameState *gameState = getGameState(memory);

	gameState->glProgram = buildProgram(memory);
	glUseProgram(gameState->glProgram);

	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	resizeView(memory);
	scaleView(gameState);
}

void checkShaders(GameMemory* memory) {
	unsigned amountOfShaders = sizeof(memory->shaderFileNames)
			/ sizeof(memory->shaderFileNames[0]);
	bool rebuildProgram = false;
	for (unsigned i = 0; i < amountOfShaders; i++) {
		if (memory->shaderModTimes[i][0] != memory->shaderModTimes[i][1]) {
			rebuildProgram = true;
			break;
		}
	}
	if (rebuildProgram) {
		initOpenGL(memory);
	}
}

GameState* beginFrame(GameMemory *memory, Input *input) {
	GameState *gameState = getGameState(memory);
	gameState->frameCounter++;

	checkShaders(memory);

	if (memory->doResize) {
		resizeView(memory);
	}

	Render::clearScreen(0);
	gameState->renderGroup.count = 0;

	checkInputForClicks(input);

	if (input->plusKeyPressed) {
		gameState->scale += 0.01;
	} else if (input->minusKeyPressed) {
		gameState->scale -= 0.01;
	}

	Render::pushEnableScaling(gameState, true, -1);

	return gameState;
}

void extractFileName(std::string fileName, std::string fileExtension,
		char result[]) {
	fileName = fileName.substr(0, fileName.size() - fileExtension.size());

	int index = 0;
	for (unsigned i = 0; i < fileName.size(); i++) {
		if (fileName[i] == '/') {
			index = 0;
			continue;
		}
		result[index] = fileName[i];
		index++;
	}
	result[index] = '\0';
}

void generateWorld(GameMemory *memory) {
	GameState *gameState = getGameState(memory);
	memory->log("Generating game world.");
	for (unsigned y = 0; y < gameState->world.height; y++) {
		for (unsigned x = 0; x < gameState->world.width; x++) {
			Tile tile = { };
			tile.orientation = Math::Vector2f(0, 1);
			tile.rect.position.x = ((int) x) * 2.0f / 10.0f - 1.0f;
			tile.rect.position.y = ((int) y) * 2.0f / 10.0f - 0.8f;
			tile.rect.size = Math::Vector2f(0.2f, 0.2f);
			tile.traversable = x % 2 != 0 || y % 2 != 0;

			gameState->world.tiles[y * gameState->world.width + x] = tile;
//			memory->log("X: %d, Y: %d, Traversable: %d", x, y, tile.traversable);
		}
	}
}

void checkPlayerTileCollision(Player *player, Tile *tile) {
	Math::Rectangle playerRect = { };
	playerRect.position = player->position;
	playerRect.size = player->size;
	//	Math::Rectangle collisionBox = getCollisionBox(tile->rect, playerRect);
	// TODO use actual size of car instead of just the position
	if (Collision::rectangle(tile->rect, player->position)) {
//		memory->log("Collision!");
	}
}

float calculateTextLength(GameState *gameState, std::string text,
		Render::FontSize fontSize) {
	if (text.size() == 0) {
		return 0.0f;
	}

	float length = 0.0f;

	for (unsigned characterIndex = 0; characterIndex < text.size();
			++characterIndex) {
		Render::Character *c = getCharacter(gameState, text[characterIndex],
				fontSize);

		length += c->advance
				+ c->kerning[text[characterIndex + 1] - Render::firstCharacter];
	}
	return length;
}

void logTimeDifferenceInMS(GameMemory *memory, long *start, std::string msg) {
	long temp = memory->queryTime();
	float diff = (float) (temp - *start) / 1000000.0f;
	memory->log(msg + ": " + std::to_string(diff));
	*start = temp;
}
