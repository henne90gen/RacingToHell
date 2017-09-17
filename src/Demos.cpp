void textDemo(GameMemory* memory, Input* input) {
	static char fontFileNames[3][100] = { "./res/font/DejaVuSansMono.ttf",
			"./res/font/ComicSans.ttf", "./res/font/Arial.ttf" };
	static int fontIndex = 0;

	if (input->enterKeyClicked) {
		loadFont(memory, fontFileNames[fontIndex]);
		fontIndex++;
		if (fontIndex >= 3) {
			fontIndex = 0;
		}
	}

	GameState *gameState = getGameState(memory);

	Math::Rectangle rect = { };
	rect.position = Math::Vector2f(-memory->aspectRatio, 0.5);
	rect.size = Math::Vector2f(memory->aspectRatio * 2.0f, 1.0);
	Render::pushRectangle(gameState, rect, 0x00ff00ff, AtomPlane::MENU);

	std::string text =
			"The quick brown fox jumps over the lazy dog The quick brown fox jumps over the lazy dog";
	Math::Vector2f position = Math::Vector2f(-1.7f, 0.3);
	Render::pushText(gameState, text, position, Render::FontSize::Small,
			0x0000ffff, AtomPlane::MENU);

	position = Math::Vector2f(-1.7f, 0);
	Render::pushText(gameState, text, position, Render::FontSize::Medium,
			0x0000ffff, AtomPlane::MENU);

	position = Math::Vector2f(-1.7f, -0.35);
	Render::pushText(gameState, text, position, Render::FontSize::Large,
			0x0000ffff, AtomPlane::MENU);
}

void performanceDemo(GameMemory *memory, AtomType type) {
	GameState *gameState = getGameState(memory);

	AtomPlane plane = AtomPlane::AI;

	switch (type) {
	case AtomType::RECTANGLE: {
		Math::Rectangle dim = { };
		dim.position = Math::Vector2f(0, 0);
		dim.size = Math::Vector2f(1, 1);
		uint32_t color = 0xff0000ff;
		Render::pushRectangle(gameState, dim, color, plane);
	}
		break;
	case AtomType::TEXTURE: {
		Render::Texture *texture = &gameState->resources.playerCarTextures[0];
		Math::Vector2f pos = Math::Vector2f(0, 0);
		Math::Vector2f size = Math::Vector2f(1, 1);
		Math::Vector2f dir = Math::Vector2f(1, 1);
		Render::pushTexture(gameState, texture, pos, size, dir, plane);
	}
		break;
	case AtomType::TRIANGLE:
		break;
	case AtomType::CIRCLE:
		break;
	case AtomType::TEXT: {
		Math::Vector2f position = Math::Vector2f(-1.7f, 0.7);
		std::string msg = "The quick brown fox jumps over the lazy dog";
		Render::pushText(gameState, msg, position, Render::FontSize::Medium,
				0xff0000ff, plane);
	}
		break;
	}
}

void performanceDemo(GameMemory *memory, AtomType type, int n) {
	for (int i = 0; i < n; i++) {
		performanceDemo(memory, type);
	}
}
