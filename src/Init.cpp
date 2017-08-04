#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library fontLibrary;
FT_Face face;

void loadCharacter(GameMemory* memory, char loadCharacter, int fontSize) {
	float scale = 0.0065f; // this is a constant to scale from pixel sizes to coordinate sizes

	int currentGlyphIndex = FT_Get_Char_Index(face, loadCharacter);
	int error = FT_Load_Glyph(face, currentGlyphIndex, FT_LOAD_RENDER);
	if (error) {
		memory->abort(
				"Couldn't load glyph for " + std::to_string(loadCharacter));
	}

	bool useKerning = FT_HAS_KERNING(face);

	Render::Character newCharacter = { };
	newCharacter.value = loadCharacter;

	newCharacter.width = face->glyph->bitmap.width;
	newCharacter.height = face->glyph->bitmap.rows;

	newCharacter.size = Math::Vector2f(newCharacter.width, newCharacter.height)
			* scale;

	newCharacter.advance = (useKerning ? face->glyph->advance.x >> 6 : 0)
			* scale;

	for (char nextChar = Render::firstCharacter;
			nextChar < Render::lastCharacter; nextChar++) {
		int nextGlyphIndex = FT_Get_Char_Index(face, nextChar);

		FT_Vector kerning;
		FT_Get_Kerning(face, currentGlyphIndex, nextGlyphIndex,
				FT_KERNING_DEFAULT, &kerning);

		newCharacter.kerning[nextChar - Render::firstCharacter] = (kerning.x
				>> 6) * scale;
	}
	unsigned bitmapSizeInPixel = newCharacter.width * newCharacter.height;

	void* content = reserveTemporaryMemory(memory, bitmapSizeInPixel * 4);
	uint8_t* src = face->glyph->bitmap.buffer;
	uint32_t* dest = (uint32_t*) content;

	for (unsigned i = 0; i < bitmapSizeInPixel; i++) {
		uint8_t alpha = *src;
		*dest = (alpha << 24);
		src++;
		dest++;
	}

	glGenTextures(1, &newCharacter.texture.id);
	glBindTexture(GL_TEXTURE_2D, newCharacter.texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newCharacter.width,
			newCharacter.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, content);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap (GL_TEXTURE_2D);

	GameState *gameState = getGameState(memory);
	gameState->resources.characterMap[fontSize][loadCharacter
			- Render::firstCharacter] = newCharacter;

	freeTemporaryMemory(memory);
}

void loadFont(GameMemory* memory, std::string fontFileName) {
	int error;
	error = FT_Init_FreeType(&fontLibrary);
	if (error) {
		memory->abort("Couldn't initialize font library.");
	}

	File fontFile = memory->readFile(fontFileName);
	error = FT_New_Memory_Face(fontLibrary, (const FT_Byte *) fontFile.content,
			fontFile.size, 0, &face);
	if (error) {
		memory->abort("Couldn't load font " + fontFileName + ".");
	}

	GameState *gameState = getGameState(memory);

	for (unsigned fontSizeIndex = 0;
			fontSizeIndex < sizeof(gameState->resources.availableFontSizes) / 4;
			fontSizeIndex++) {
		int fontSize = getFontSize(gameState, fontSizeIndex);
//		int error = FT_Set_Char_Size(face, 0, fontSize*32, WINDOW_WIDTH, WINDOW_HEIGHT);
		int error = FT_Set_Pixel_Sizes(face, fontSize * 2, 0);
		if (error) {
			std::string message = "Couldn't set pixel size to "
					+ std::to_string(fontSize) + ".";
			memory->abort(message);
		}

		int count = 0;
		for (char currentChar = Render::firstCharacter;
				currentChar < Render::lastCharacter; currentChar++) {
			loadCharacter(memory, currentChar, fontSizeIndex);
			count++;
		}
	}
}

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, content);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

	gameState->resources.bulletTexture = loadTexture(memory,
			"./res/textures/bullet.bmp");

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
	gameState->bulletSpeed = 0.005f;

	gameState->lastAIBulletIndex = -1;
	gameState->lastPlayerBulletIndex = -1;

	gameState->world = {};
	gameState->world.lastItemIndex = -1;
	gameState->world.lastTrafficCarIndex = -1;
	gameState->world.width = 10;
	gameState->world.height = 10;

	gameState->isInBossFight = true;
	loadBoss(gameState);

	gameState->menuState = MenuState::MAIN; // triggers world generation when loading the game directly into game mode
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
	glDepthMask (GL_TRUE);
	glEnable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	resizeView(memory, 1.0);

	loadFont(memory, "./res/font/arial.ttf");

	loadAudioClips(memory);

	loadTextures(memory);

	Sound::output(gameState, &gameState->resources.level1Music, 0.1f, 0.1f,
			Sound::PLAY_LOOP);

	resetGameState(gameState);
}

