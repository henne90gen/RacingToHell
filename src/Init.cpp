#if FONT_ENABLED
#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library fontLibrary;
#endif

/**
 * Moves content to graphics memory.
 */
void loadTextureToGraphicsMemory(Render::Texture *texture, void *content)
{
	if (!texture->id)
	{
		glGenTextures(1, &texture->id);
	}

	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, content);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}

#if FONT_ENABLED
/**
 * Load the texture and kerning information for the specified character
 */
void loadCharacter(GameMemory *memory, FT_Face face, char loadCharacter,
				   int fontSize, bool hasKerning)
{
	float scale = 0.002f; // this is a constant to scale from pixel sizes to coordinate sizes

	int currentGlyphIndex = FT_Get_Char_Index(face, loadCharacter);
	int error = FT_Load_Glyph(face, currentGlyphIndex, FT_LOAD_RENDER);
	if (error)
	{
		memory->abort(
			"Couldn't load glyph for " + std::to_string(loadCharacter));
	}

	GameState *gameState = getGameState(memory);

	Render::Character newCharacter =
		gameState->resources.characterMap[fontSize][loadCharacter - Render::firstCharacter];

	newCharacter.value = loadCharacter;
	newCharacter.hasKerning = hasKerning;

	newCharacter.texture.width = face->glyph->bitmap.width;
	newCharacter.texture.height = face->glyph->bitmap.rows;
	newCharacter.texture.xDivision = 1;
	newCharacter.texture.yDivision = 1;

	newCharacter.size = glm::vec2(newCharacter.texture.width,
									   newCharacter.texture.height) *
						scale;

	newCharacter.bearing = glm::vec2(face->glyph->bitmap_left,
										  (newCharacter.texture.height - face->glyph->bitmap_top)) *
						   scale;
	newCharacter.bearing.y *= -1;

	newCharacter.advance = (face->glyph->advance.x >> 6) * scale;

	if (hasKerning)
	{
		for (char nextChar = Render::firstCharacter;
			 nextChar < Render::lastCharacter; nextChar++)
		{
			int nextGlyphIndex = FT_Get_Char_Index(face, nextChar);

			FT_Vector kerning;
			FT_Get_Kerning(face, currentGlyphIndex, nextGlyphIndex,
						   FT_KERNING_DEFAULT, &kerning);

			newCharacter.kerning[nextChar - Render::firstCharacter] = (kerning.x >> 6) * scale;
		}
	}

	unsigned bitmapSizeInPixel = newCharacter.texture.width * newCharacter.texture.height;

	void *content = reserveTemporaryMemory(memory, bitmapSizeInPixel * 4);
	uint8_t *src = face->glyph->bitmap.buffer;
	uint32_t *dest = (uint32_t *)content;

	for (unsigned i = 0; i < bitmapSizeInPixel; i++)
	{
		*dest = (*src << 24) + 0x00ffffff;
		src++;
		dest++;
	}

	loadTextureToGraphicsMemory(&newCharacter.texture, content);

	freeTemporaryMemory(memory);

	gameState->resources.characterMap[fontSize][loadCharacter - Render::firstCharacter] = newCharacter;
}

/**
 * Load the specified font file to be used as game font
 */
void loadFont(GameMemory *memory, std::string fontFileName)
{
	int error;
	GameState *gameState = getGameState(memory);

	File fontFile = memory->readFile(fontFileName);
	FT_Face face = {};
	error = FT_New_Memory_Face(fontLibrary, (const FT_Byte *)fontFile.content,
							   fontFile.size, 0, &face);
	if (error)
	{
		memory->abort(
			"Couldn't load font " + fontFileName + ". Errorcode: " + std::to_string(error));
	}

	extractFileName(fontFileName, ".ttf", gameState->resources.fontName);

	bool hasKerning = FT_HAS_KERNING(face);

	for (unsigned fontSizeIndex = 0;
		 fontSizeIndex < sizeof(gameState->resources.availableFontSizes) / 4;
		 fontSizeIndex++)
	{
		int fontSize = getFontSize(gameState, fontSizeIndex);
		int error = FT_Set_Pixel_Sizes(face, fontSize * 5, 0);
		if (error)
		{
			std::string message = "Couldn't set pixel size to " + std::to_string(fontSize) + ".";
			memory->abort(message);
		}

		for (char currentChar = Render::firstCharacter;
			 currentChar < Render::lastCharacter; currentChar++)
		{
			loadCharacter(memory, face, currentChar, fontSizeIndex, hasKerning);
		}
	}
}
#endif

/**
 * Load all audio clips that are going to be used into memory
 */
void loadAudioClips(GameMemory *memory)
{
	GameState *gameState = getGameState(memory);
	gameState->resources.aiShot = Sound::loadWAV(memory,
												 "./res/sound/shotAI.wav");
	gameState->resources.playerShot = Sound::loadWAV(memory,
													 "./res/sound/shotPlayer.wav");
	gameState->resources.level1Music = Sound::loadWAV(memory,
													  "./res/sound/music/level1.wav");
}

/**
 * Load a texture from a bmp file and pushes it to the graphics card
 */
Render::Texture loadTexture(GameMemory *memory, std::string fileName,
							int xDivision = 1, int yDivision = 1)
{
	File file = memory->readFile(fileName);
	if (((char *)file.content)[0] != 'B' || (file.content)[1] != 'M')
	{
		memory->abort(file.name + " is not a bitmap file.");
	}
	int fileHeaderSize = 14;
	BitmapHeader header = *((BitmapHeader *)(file.content + fileHeaderSize));

	if (header.bitsPerPixel != 32)
	{
		memory->abort("Image must have 32-bit of color depth.");
	}

	Render::Texture texture = {};
	texture.width = header.width;
	texture.height = header.height;
	texture.bytesPerPixel = header.bitsPerPixel / 8;
	texture.xDivision = xDivision;
	texture.yDivision = yDivision;
	void *content = reserveTemporaryMemory(memory,
										   texture.width * texture.height * texture.bytesPerPixel);

	importPixelData(file.content + header.size + fileHeaderSize, content,
					header.width, header.height, 0, 0, texture.width, texture.height);

	memory->freeFile(&file);

	loadTextureToGraphicsMemory(&texture, content);

	freeTemporaryMemory(memory);

	return texture;
}

/**
 * Load all textures that are going to be used into memory
 */
void loadTextures(GameMemory *memory)
{
	GameState *gameState = getGameState(memory);

	gameState->resources.bulletTexture = loadTexture(memory,
													 "./res/textures/bullet.bmp");

	for (int i = 0; i < 4; i++)
	{
		std::string fileName = "./res/textures/roads/road" + std::to_string(i) + ".bmp";
		gameState->resources.roadTextures[i] = loadTexture(memory, fileName);
	}

	for (unsigned i = 0; i < NUM_PLAYER_TEXTURES; i++)
	{
		std::string playerSprite = "./res/textures/cars/player" + std::to_string(i) + ".bmp";
		gameState->resources.playerCarTextures[i] = loadTexture(memory,
																playerSprite);
	}

	for (unsigned i = 0; i < NUM_TRAFFIC_TEXTURES; i++)
	{
		std::string carSprite = "./res/textures/cars/traffic" + std::to_string(i) + ".bmp";
		gameState->resources.trafficCarTextures[i] = loadTexture(memory,
																 carSprite);
	}

	gameState->resources.itemTextures[TOOLBOX_ID] = loadTexture(memory,
																"./res/textures/toolbox.bmp");

	gameState->resources.itemTextures[CANISTER_ID] = loadTexture(memory,
																 "./res/textures/canister.bmp");

	// Explosion sprite
	gameState->resources.explosion = loadTexture(memory,
												 "./res/textures/explosion.bmp", 9, 9);

	// Tank boss
	gameState->resources.tank = loadTexture(memory,
											"./res/textures/bosses/tank.bmp");
	gameState->resources.tankCannon = loadTexture(memory,
												  "./res/textures/bosses/tank_cannon.bmp");
}

/**
 * Resets everything in the GameState to their initial values
 */
void resetGameState(GameState *gameState)
{
	gameState->player = {};
	gameState->player.position = glm::vec2(0, 0);
	gameState->player.size = glm::vec2(0.05f, 0.10f);
	//	gameState->player.direction = glm::vec2(1, 0);
	// TODO balance speed
	//	gameState->player.speed = 0;
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
}

/**
 * Initialize the game appropriately
 */
void init(GameMemory *memory)
{
	std::srand(1337);
	memory->isInitialized = true;

	GameState *gameState = (GameState *)reservePermanentMemory(memory,
															   sizeof(GameState));

	*gameState = {};

	char vertexShaderFileName[] = "./res/shaders/vertex.glsl";
	for (unsigned i = 0; i < sizeof(vertexShaderFileName) / sizeof(char); i++)
	{
		memory->shaderFileNames[0][i] = vertexShaderFileName[i];
	}
	char fragmentShaderFileName[] = "./res/shaders/fragment.glsl";
	for (unsigned i = 0; i < sizeof(fragmentShaderFileName) / sizeof(char);
		 i++)
	{
		memory->shaderFileNames[1][i] = fragmentShaderFileName[i];
	}

	glClearColor(1.0, 0, 1.0, 1.0);

	// setting up OpenGL
	gameState->scale = 1.0f;
	gameState->rotationAngle = 0.0f;
	initOpenGL(memory);

#if FONT_ENABLED
	// setting up font system
	int error = FT_Init_FreeType(&fontLibrary);
	if (error)
	{
		memory->abort("Couldn't initialize font library.");
	}
	loadFont(memory, "./res/font/Arial.ttf");
#endif

	loadAudioClips(memory);

	loadTextures(memory);

	Sound::output(gameState, &gameState->resources.level1Music, 0.1f, 0.1f,
				  Sound::PLAY_LOOP);

	resetGameState(gameState);
	loadMenu(memory, MenuState::MAIN);
}
