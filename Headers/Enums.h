#pragma once

enum class GameState {
    Loading,
    LoadingToMain,
    MainMenu,
    Running,
    Pause,
    Highscores,
    Options,
    About,
    BossFight,
    LevelUp,
    GameOver,
    Exiting,
    RunningMultiplayer,
    PauseMultiplayer,
    GameOverMultiplayer,
    BossFightMultiplayer,
    MultiplayerSelection,
    Lobby,
    Connecting,
    Countdown
};

enum class GameMode {
    Standard, Invincible, InfEnergy, Hardcore
};

// TODO give them real names
// HACK using last element for counting (http://stackoverflow.com/questions/2102582/how-can-i-count-the-items-in-an-enum)
enum class PlayerCarIndex {
    Car1, Car2, Car3, Car4, Car5, Car6, NumberOfCars
};

enum class Key {
    Up = 1 << 0,
    Right = 1 << 1,
    Down = 1 << 2,
    Left = 1 << 3
};

enum class ScoreEvent {
    Tick, DestroyedCar, DefeatedBoss
};