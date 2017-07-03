void updateTank(GameState *gameState) {
	Boss *boss = &gameState->boss;
	Render::Texture *tankTexture = &gameState->resources.tank;

	if (boss->health < 0) {
//		levelUp(gameState);
	}

	switch (boss->currentPhase) {
	case 0: // Shoot and drive left and right
		boss->position = boss->position + Math::Vector2f( { boss->speed, 0 });
		if (boss->position.x + tankTexture->width / 2 > WINDOW_WIDTH) {
			boss->speed = -boss->speed;
		} else if (boss->position.x - tankTexture->width / 2 < 0) {
			boss->speed = -boss->speed;
		}
		if (gameState->frameCounter % boss->bulletFrequency == 0) {
			spawnBullet(gameState, boss->position, Math::Vector2f( { 0,
					gameState->bulletSpeed }), false);
		}
		break;
	case 1: //

		break;
	}
}

void updateBoss(GameState *gameState) {
	switch (gameState->boss.type) {
	case BossType::TANK:
		updateTank(gameState);
		break;
	case BossType::MECH:
		break;
	}
}

void renderBoss(VideoBuffer *buffer, GameState *gameState) {
	Render::Texture* tankTexture = &gameState->resources.tank;
	int tankOffsetX = gameState->boss.position.x - tankTexture->width / 2;
	int tankOffsetY = gameState->boss.position.y - tankTexture->height / 2;
	Render::textureAlpha(buffer, tankTexture, tankOffsetX, tankOffsetY);

	Render::Texture* tankCannonTexture = &gameState->resources.tankCannon;
	int cannonOffsetX = gameState->boss.position.x
			- tankCannonTexture->width / 2;
	int cannonOffsetY = gameState->boss.position.y
			- tankCannonTexture->height / 2 + 29;
	Render::textureAlpha(buffer, tankCannonTexture, cannonOffsetX,
			cannonOffsetY);
}

void updateAndRenderBoss(VideoBuffer *buffer, GameState *gameState,
		bool update) {
	if (update) {
		updateBoss(gameState);
	}
	if (gameState->menuState == MenuState::GAME
			|| gameState->menuState == MenuState::PAUSE) {
		renderBoss(buffer, gameState);
	}
}

void loadBoss(GameState *gameState) {
	gameState->boss = {};
	switch (gameState->level % 2) {
		case 0:
		gameState->boss.type = BossType::TANK;
		gameState->boss.speed = 2.5f;
		gameState->boss.bulletFrequency = 60;
		break;
		case 1:
		gameState->boss.type = BossType::MECH;
		break;
	}
	gameState->boss.position = {WINDOW_WIDTH / 2, 150};
}
