#pragma once

enum class GameState {
	Running, Main, Pause, Highscores, Options, About, BossFight, LevelUp, GameOver, Loading, Exiting, 
	RunningMultiplayer, PauseMultiplayer, GameOverMultiplayer, BossFightMultiplayer, MultiplayerSelection, Lobby, Connecting, Countdown
};

enum class GameMode {
	Standard, Invincible
};
