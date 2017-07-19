/**
 * Loads all audio clips that are going to be used into memory
 */
void loadAudioClips(GameMemory* memory) {
	GameState *gameState = getGameState(memory);
	gameState->resources.aiShot = Sound::loadWAV(memory,
			"./res/sound/shotAI.wav");
	gameState->resources.playerShot = Sound::loadWAV(memory,
			"./res/sound/shotPlayer.wav");
	gameState->resources.level1Music = Sound::loadWAV(memory,
			"./res/sound/music/level1.wav");
}

/**
 * Loads a texture from a bmp file and pushes it to the graphics card
 */
Render::Texture loadTexture(GameMemory *memory, std::string fileName) {
	File file = memory->readFile(fileName);
	if (((char*) file.content)[0] != 'B' || (file.content)[1] != 'M') {
		memory->abort(file.name + " is not a bitmap file.");
	}
	int fileHeaderSize = 14;
	BitmapHeader header = *((BitmapHeader*) (file.content + fileHeaderSize));

	if (header.bitsPerPixel != 32) {
		memory->abort("Image must have 32-bit of color depth.");
	}

	Render::Texture texture = { };
	texture.width = header.width;
	texture.height = header.height;
	texture.bytesPerPixel = header.bitsPerPixel / 8;
	void* content = reserveTemporaryMemory(memory,
			texture.width * texture.height * texture.bytesPerPixel);

	importPixelData(file.content + header.size + fileHeaderSize, content,
			header.width, header.height, 0, 0, texture.width, texture.height);

	memory->freeFile(&file);

	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, content);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap (GL_TEXTURE_2D);

	freeTemporaryMemory(memory);

	return texture;
}

/**
 * Loads all textures that are going to be used into memory
 */
void loadTextures(GameMemory *memory) {
	GameState *gameState = getGameState(memory);

	for (int i = 0; i < 4; i++) {
		std::string fileName = "./res/textures/roads/road" + std::to_string(i)
				+ ".bmp";
		gameState->resources.roadTextures[i] = loadTexture(memory, fileName);
	}

	for (unsigned i = 0; i < NUM_PLAYER_TEXTURES; i++) {
		std::string playerSprite = "./res/textures/cars/player"
				+ std::to_string(i) + ".bmp";
		gameState->resources.playerCarTextures[i] = loadTexture(memory,
				playerSprite);
	}

	for (unsigned i = 0; i < NUM_TRAFFIC_TEXTURES; i++) {
		std::string carSprite = "./res/textures/cars/traffic"
				+ std::to_string(i) + ".bmp";
		gameState->resources.trafficCarTextures[i] = loadTexture(memory,
				carSprite);
	}

	gameState->resources.itemTextures[TOOLBOX_ID] = loadTexture(memory,
			"./res/textures/toolbox.bmp");

	gameState->resources.itemTextures[CANISTER_ID] = loadTexture(memory,
			"./res/textures/canister.bmp");

	// Explosion sprite
	gameState->resources.explosion = loadTexture(memory,
			"./res/textures/explosion.bmp");

	// Tank boss
	gameState->resources.tank = loadTexture(memory,
			"./res/textures/bosses/tank.bmp");
	gameState->resources.tankCannon = loadTexture(memory,
			"./res/textures/bosses/tank_cannon.bmp");
}

/**
 * Resets everything in the GameState to their initial values
 */
void resetGameState(GameState *gameState) {
	gameState->player = {};
	gameState->player.position = Math::Vector2f(0, 0);
	gameState->player.size = Math::Vector2f(0.05f, 0.10f);
	gameState->player.direction = Math::Vector2f(1, 0);
	// TODO balance speed
	gameState->player.speed = 0;
	gameState->player.maxSpeed = PLAYER_SPEED;
	// TODO balance health
	gameState->player.health = 10;
	gameState->player.maxHealth = 100;
	// TODO balance energy
	gameState->player.energy = 1000;
	gameState->player.maxEnergy = 1000;

	gameState->level = 0;
	gameState->levelTime = 0;
	gameState->maxLevelTime = 60;
	gameState->difficulty = 1;
	gameState->roadPosition = 0;
	gameState->frameCounter = 0;

	// TODO balance spawn rates
	gameState->trafficFrequency = 50;
	gameState->bulletFrequency = 50;
	gameState->itemFrequency = 50;
	gameState->bulletSpeed = 7.5f;

	gameState->lastAIBulletIndex = -1;
	gameState->lastItemIndex = -1;
	gameState->lastPlayerBulletIndex = -1;
	gameState->lastTrafficCarIndex = -1;

	gameState->isInBossFight = true;
	loadBoss(gameState);

	loadMenu(gameState, MenuState::GAME);
}

/**
 * Initialize the game memory appropriately
 */
void init(GameMemory *memory) {
	std::srand(time(0));
	memory->isInitialized = true;

	GameState *gameState = (GameState *) reservePermanentMemory(memory,
			sizeof(GameState));

	*gameState = {};

	// setting up OpenGL
	gameState->glProgram = buildProgram(memory);
	glUseProgram(gameState->glProgram);
	float scale = 4;
	static GLfloat scaleMatrix[16] = { 9.0 / 16.0 * scale, 0, 0, 0, //
			0, 1.0 * scale, 0, 0, //
			0, 0, 1.0, 0, //
			0, 0, 0, 1.0 };
	static GLuint scaleMatrixLocation = glGetUniformLocation(
			gameState->glProgram, "u_ScaleMatrix");
	glUniformMatrix4fv(scaleMatrixLocation, 1, GL_FALSE, &scaleMatrix[0]);

	resetGameState(gameState);

	Text::loadFont(memory, "./res/font/arial.ttf");

	loadAudioClips(memory);

	loadTextures(memory);

	Sound::output(gameState, &gameState->resources.level1Music, 0.1f, 0.1f,
			Sound::PLAY_LOOP);
}

