void shootAtPlayer(GameState *gameState, Math::Vector2f origin) {
	Math::Vector2f direction = gameState->player.position - origin;
	spawnBullet(gameState, origin, direction, false);
}

void updateTank(GameState *gameState) {
	Boss *boss = &gameState->boss;
	Render::Texture *tankTexture = &gameState->resources.tank;
	static int counter = 0;
	static bool shooting = false;

	// TODO change triggers for the different attack styles
	if (boss->health < 0) {
//		levelUp(gameState);
	} else if (boss->health < boss->maxHealth * 0.3) {
		boss->currentPhase = 2;
	} else if (boss->health < boss->maxHealth * 0.6) {
		boss->currentPhase = 1;
	} else {
		boss->currentPhase = 0;
	}

	boss->currentPhase = 2;

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
	case 1: // Three bullet burst
	{
		Math::Vector2f centerPosition = Math::Vector2f(

		{ WINDOW_WIDTH / 2, 150 });
		if (boss->speed < 0) {
			boss->speed = -boss->speed;
		}
		if (boss->position.x != centerPosition.x
				|| boss->position.y != centerPosition.y) {
			Math::Vector2f movement = centerPosition - boss->position;
			movement = Math::normalize(movement);
			movement = movement * boss->speed;
			boss->position = boss->position + movement;
		}
		if (gameState->frameCounter % 5 == 0) {
			if (counter == -3) {
				shooting = true;
				counter = 0;
			} else if (counter == 3) {
				shooting = false;
			}
			if (counter < 3 && shooting) {
				counter++;
				shootAtPlayer(gameState, boss->position);
			} else {
				counter--;
			}
		}
	}
		break;
	case 2: // Waves of bullets
		// TODO maybe move the frequency to a variable
		if (gameState->frameCounter % 60 == 0) {
			Math::Vector2f playerDirection = gameState->player.position
					- boss->position;
			float angle = Math::angle(playerDirection) + PI / 16;
			for (int i = 0; i < 5; i++) {
				Math::Vector2f direction = Math::Vector2f(angle - PI / 32 * i);
				spawnBullet(gameState, boss->position, direction, false);
			}
		}
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
