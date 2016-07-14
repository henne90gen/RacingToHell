#pragma once

enum class GameState {
	Running, Main, Pause, Highscores, Options, About, BossFight, LevelUp, GameOver, Loading, Exiting, 
	RunningMultiplayer, PauseMultiplayer, GameOverMultiplayer, BossFightMultiplayer, MultiplayerSelection, Lobby, Connecting, Countdown
};

enum class GameMode {
	Standard, Invincible, InfEnergy, Hardcore
};

enum class KeyDown {
	Up = 1 << 1,
	Right = 1 << 2,
	Down = 1 << 3,
	Left = 1 << 4
};