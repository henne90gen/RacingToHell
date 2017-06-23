/**
 * Loads all audio clips that are going to be used into memory
 */
void loadAudioClips(GameMemory* memory) {
	GameState *gameState = getGameState(memory);
	gameState->resources.AIShot = Sound::loadWAV(memory,
			"./res/sound/shotAI.wav");
	gameState->resources.playerShot = Sound::loadWAV(memory,
			"./res/sound/shotPlayer.wav");
	gameState->resources.Level1Music = Sound::loadWAV(memory,
			"./res/sound/music/level1.wav");
}

/**
 * Loads all textures that are going to be used into memory
 */
void loadTextures(GameMemory *memory) {
	GameState *gameState = getGameState(memory);
	for (int i = 0; i < 4; i++) {
		std::string filename = "./res/textures/roads/road" + std::to_string(i)
				+ ".bmp";
		File file = memory->readFile(filename);
		gameState->resources.roadTextures[i] = readBmpIntoMemory(file, memory);
		memory->freeFile(&file);
	}

	for (unsigned i = 0; i < NUM_PLAYER_TEXTURES; i++) {
		std::string carSprites = "./res/textures/cars/player"
				+ std::to_string(i) + ".bmp";
		File carFile = memory->readFile(carSprites);
		gameState->resources.playerCarTextures[i] = readBmpIntoMemory(carFile,
				memory);
		memory->freeFile(&carFile);
	}

	for (unsigned i = 0; i < NUM_TRAFFIC_TEXTURES; i++) {
		std::string carSprites = "./res/textures/cars/traffic"
				+ std::to_string(i) + ".bmp";
		File carFile = memory->readFile(carSprites);
		gameState->resources.trafficCarTextures[i] = readBmpIntoMemory(carFile,
				memory);
		memory->freeFile(&carFile);
	}

	File toolFile = memory->readFile("./res/textures/toolbox.bmp");
	gameState->resources.itemTextures[TOOLBOX_ID] = readBmpIntoMemory(toolFile,
			memory);
	memory->freeFile(&toolFile);

	File canFile = memory->readFile("./res/textures/canister.bmp");
	gameState->resources.itemTextures[CANISTER_ID] = readBmpIntoMemory(canFile,
			memory);
	memory->freeFile(&canFile);

	File explosionFile = memory->readFile("./res/textures/explosion.bmp");
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			int offsetX = x * 100;
			int offsetY = y * 100;
			int width = 100;
			int height = 100;
			gameState->resources.explosion[y * 9 + x] = readBmpIntoMemory(
					explosionFile, memory, offsetX, offsetY, width, height);
		}
	}
	memory->freeFile(&explosionFile);
}

/**
 * Resets everything in the GameState to their initial values
 */
void resetGameState(GameState *gameState) {
	gameState->player = {};
	gameState->player.position.x = (float) (WINDOW_WIDTH / 2);
	gameState->player.position.y = (float) (WINDOW_HEIGHT / 2);
	// TODO balance speed
	gameState->player.speed = 10;
	// TODO balance health
	gameState->player.health = 10;
	gameState->player.maxHealth = 100;
	// TODO balance energy
	gameState->player.energy = 1000;
	gameState->player.maxEnergy = 1000;

	loadMainMenu(&gameState->currentMenu);
	gameState->level = 0;
	gameState->levelTime = 0;
	gameState->maxLevelTime = 60;
	gameState->difficulty = 0;
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

	// FIXME what is lastPlayingSound?
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
	resetGameState(gameState);

	Text::loadFont(memory, "./res/font/arial.ttf");

	loadAudioClips(memory);

	loadTextures(memory);

	Sound::output(gameState, &gameState->resources.Level1Music, 0.1f, 0.1f,
			Sound::PLAY_LOOP);
}

