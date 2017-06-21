void loadTextures(GameMemory *memory, GameState *gameState) {
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

	// Loading Toolbox with index 0
	File toolFile = memory->readFile("./res/textures/toolbox.bmp");
	gameState->resources.itemTextures[TOOLBOX_ID] = readBmpIntoMemory(toolFile,
			memory);
	memory->freeFile(&toolFile);

	// Loading Canister with index 1
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
			bool output = y == 8 && x == 8;
			gameState->resources.explosion[y * 9 + x] = readBmpIntoMemory(
					explosionFile, memory, offsetX, offsetY, width, height,
					output);
		}
	}
	memory->freeFile(&explosionFile);
}

void resetGameState(GameState *gameState) {
	gameState->player = {};
	gameState->player.position.x = (float) (WINDOW_WIDTH / 2);
	gameState->player.position.y = (float) (WINDOW_HEIGHT / 2);
	gameState->player.speed = 10;
	gameState->player.health = 75;
	gameState->player.maxHealth = 75;
	gameState->player.energy = 75;
	gameState->player.maxEnergy = 75;

	gameState->currentMenu = getMainMenu();
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

void init(GameMemory *memory) {
	std::srand(time(0));
	memory->isInitialized = true;

	GameState *gameState = (GameState *) reservePermanentMemory(memory,
			sizeof(GameState));

	*gameState = {};
	resetGameState(gameState);

	Text::loadFont(memory, "./res/font/arial.ttf");

	gameState->resources.AIShot = Sound::loadWAV(memory,
			"./res/sound/shotAI.wav");
	gameState->resources.playerShot = Sound::loadWAV(memory,
			"./res/sound/shotPlayer.wav");
	gameState->resources.Level1Music = Sound::loadWAV(memory,
			"./res/sound/music/level1.wav");
	Sound::output(gameState, &gameState->resources.Level1Music, 0.1f, 0.1f,
			Sound::PLAY_LOOP);

	loadTextures(memory, gameState);
}

