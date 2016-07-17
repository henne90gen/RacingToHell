#pragma once

enum class GameState {
	Running, Main, Pause, Highscores, Options, About, BossFight, LevelUp, GameOver, Loading, Exiting, 
	RunningMultiplayer, PauseMultiplayer, GameOverMultiplayer, BossFightMultiplayer, MultiplayerSelection, Lobby, Connecting, Countdown
};

enum class GameMode {
	Standard, Invincible, InfEnergy, Hardcore
};

enum class Key {
	Up = 1 << 0,
	Right = 1 << 1,
	Down = 1 << 2,
	Left = 1 << 3
};