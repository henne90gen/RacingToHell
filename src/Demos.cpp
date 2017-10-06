#include "GameDemo.cpp"

void textDemo(GameMemory* memory, Input* input, AtomPlane plane) {
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
	Render::pushRectangle(gameState, rect, 0x00ff00ff, plane - 1);

	std::string text = "The quick brown fox jumps over the lazy dog.";
	Math::Vector2f position = Math::Vector2f(-1.7f, 0.3);
	Render::pushText(gameState, text, position, Render::FontSize::Small,
			0xff00ffff, plane);

	position = Math::Vector2f(-1.7f, 0);
	Render::pushText(gameState, text, position, Render::FontSize::Medium,
			0xff00ffff, plane);

	position = Math::Vector2f(-1.7f, -0.35);
	Render::pushText(gameState, text, position, Render::FontSize::Large,
			0xff00ffff, plane);
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
	case AtomType::TRIANGLE: {
		Math::Vector2f p1 = Math::Vector2f(1, 1);
		Math::Vector2f p2 = Math::Vector2f(-1, 1);
		Math::Vector2f p3 = Math::Vector2f();
		Render::pushTriangle(gameState, p1, p2, p3, 0xff0000ff, plane);
	}
		break;
	case AtomType::CIRCLE: {
		Math::Vector2f position = Math::Vector2f();
		Render::pushCircle(gameState, position, 1, 0xff0000ff, plane);
	}
		break;
	case AtomType::TEXT: {
		Math::Vector2f position = Math::Vector2f(-1.7f, 0.7);
		std::string msg = "The quick brown fox jumps over the lazy dog";
		Render::pushText(gameState, msg, position, Render::FontSize::Medium,
				0xff0000ff, plane);
	}
		break;
	case AtomType::NOSCALE:
	case AtomType::SCALE:
		Render::pushEnableScaling(gameState, false, plane);
		Render::pushEnableScaling(gameState, true, plane);
		break;
	}
}

void performanceDemo(GameMemory *memory, AtomType type, int n) {
	for (int i = 0; i < n; i++) {
		performanceDemo(memory, type);
	}
}

void animationDemo(GameMemory *memory, AtomPlane plane) {
	Render::Texture *explosionTexture =
			&getGameState(memory)->resources.explosion;

	static unsigned int explosionIndex3 = 0;
	Math::Vector2f position = Math::Vector2f(1, 0);
	Math::Vector2f size = Math::Vector2f(1, 1);
	Render::pushAnimation(getGameState(memory), explosionTexture, position,
			size, &explosionIndex3, plane, 3);

	static unsigned int explosionIndex2 = 0;
	if (explosionIndex2 >= explosionIndex3) {
		position = Math::Vector2f();
		size = Math::Vector2f(1, 1);
		Render::pushAnimation(getGameState(memory), explosionTexture, position,
				size, &explosionIndex2, plane, 2);
	}

	static unsigned int explosionIndex1 = 0;
	if (explosionIndex1 >= explosionIndex3) {
		position = Math::Vector2f(-1, 0);
		size = Math::Vector2f(1, 1);
		Render::pushAnimation(getGameState(memory), explosionTexture, position,
				size, &explosionIndex1, plane);
	}
}

void followingCarDemo(GameMemory *memory, Input *input) {
	GameState *gameState = getGameState(memory);

	if (input->shootKeyPressed
			&& gameState->agentCount
					< (int) sizeof(gameState->agents) / (int) sizeof(Player)) {
		gameState->agents[gameState->agentCount++] = Player();
		Player *agent = &gameState->agents[gameState->agentCount - 1];
		agent->position = input->mousePosition;
		agent->size = Math::Vector2f(0.05f, 0.10f);
		agent->maxSpeed = PLAYER_SPEED;
		agent->carIndex = std::rand() % NUM_PLAYER_TEXTURES;
	}

	for (int i = 0; i < gameState->agentCount; i++) {
		Player *agent = &gameState->agents[i];

		Render::Texture *texture =
				&gameState->resources.playerCarTextures[agent->carIndex];

		Math::Vector2f desired = input->mousePosition - agent->position;
		//		if (Math::length(desired) > 0.01f) {
		desired = Math::normalize(desired);
		desired = desired * agent->maxSpeed;

		Math::Vector2f steering = desired - agent->velocity;
		steering = Math::normalize(steering);
		steering = steering * 0.00005f;

		agent->acceleration = agent->acceleration + steering;
		//		} else {
		//			agent->velocity = Math::Vector2f();
		//			agent->acceleration = Math::Vector2f();
		//		}

		agent->velocity = agent->velocity + agent->acceleration;
		if (Math::length(agent->velocity) > agent->maxSpeed) {
			agent->velocity = Math::normalize(agent->velocity)
					* agent->maxSpeed;
		}
		agent->position = agent->position + agent->velocity;

		if (agent->velocity.x != 0.0f && agent->velocity.y != 0.0f) {
			agent->direction = agent->velocity;
		}

		Render::pushTexture(gameState, texture, agent->position, agent->size,
				agent->direction, 0, AtomPlane::PLAYER);
	}
}
