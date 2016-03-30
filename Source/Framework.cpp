#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework() : _FrameTime(0), _FPS(60.0f), _IsRunning(true), _GameState(GameState::Loading)
{
	_RenderWindow.create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32U), "Racing to Hell", sf::Style::Close);
	_RenderWindow.setMouseCursorVisible(false);

	sf::Image Icon;
	if (Icon.loadFromFile("Resources/Texture/Icon/Icon.png"))
	{
		_RenderWindow.setIcon(Icon.getSize().x, Icon.getSize().y, Icon.getPixelsPtr());
	}

	//Seed
	srand(time(NULL));
}

Framework::~Framework()
{
	_CarSkins.clear();
}

void Framework::run()
{
	while (_IsRunning)
	{
		if (measureTime()) {
			render();
		}

		handleEvents();
		
		update();

		playSounds();
	}
}

void Framework::render()
{
	if (_GameState != GameState::Loading || _LoadingScreen.isFadingAway()) {
		_Level.render(_RenderWindow);
		_GameObjectContainer.render(_RenderWindow, _GameState == GameState::Running || _GameState == GameState::BossFight);
	}

	switch (_GameState) {
	case GameState::Running:
		setMouseVisible(false);
		_HeadsUpDisplay.render(_RenderWindow);
		break;
	case GameState::Main:
		setMouseVisible(true);
		_MainMenu.render(_RenderWindow);
		break;
	case GameState::Pause:
		setMouseVisible(true);
		_PauseMenu.render(_RenderWindow);
		break;
	case GameState::Highscores:
		setMouseVisible(true);
		_HighscoreMenu.render(_RenderWindow);
		break;
	case GameState::Options:
		setMouseVisible(true);
		_OptionsMenu.render(_RenderWindow);
		break;
	case GameState::About:
		setMouseVisible(true);
		_AboutScreen.render(_RenderWindow);
		break;
	case GameState::LevelUp:
		setMouseVisible(false);
		_LevelUpScreen.render(_RenderWindow);
		break;
	case GameState::BossFight:
		setMouseVisible(false);
		_HeadsUpDisplay.render(_RenderWindow);
		break;
	case GameState::GameOver:
		setMouseVisible(true);
		_GameOverScreen.render(_RenderWindow);
		break;
	case GameState::Loading:
		setMouseVisible(false);
		if (_LoadingScreen.isFadingAway()) {
			_MainMenu.render(_RenderWindow);
		}
		_LoadingScreen.render(_RenderWindow);
		break;
	case GameState::MultiplayerSelection:
		setMouseVisible(true);
		_MultiplayerMenu.render(_RenderWindow);
		break;
	case GameState::Lobby:
		setMouseVisible(true);
		_MultiplayerLobby.render(_RenderWindow);
		break;
	}

	_RenderWindow.display();
}

void Framework::handleEvents()
{
	switch (_GameState) {
	case GameState::Running:
		while (_RenderWindow.pollEvent(_Event))
		{
			if (_Event.type == sf::Event::Closed) {
				_GameState = GameState::Exiting;
			}
			else {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 7)) {
					_GameState = GameState::Pause;
					_PauseMenu.setReturnState(GameState::Running);
				}
				else {
					_GameObjectContainer.handleEvent(_Event);
				}
			}
		}
		break;
	case GameState::Pause:
		_GameObjectContainer.stopSounds();
		_GameState = _PauseMenu.handleEvents(_RenderWindow);
		if (_GameState == GameState::Running) {
			_Clock.restart();
		}
		else if (_GameState == GameState::Main) {
			resetGame();
		}
		else if (_GameState == GameState::Options) {
			_OptionsMenu.enableDifficultySelection(false);
			_OptionsMenu.setReturnState(GameState::Pause);
		}
		break;
	case GameState::Main:
		_GameState = _MainMenu.handleEvents(_RenderWindow);
		_CurrentCarSkinIndex = _MainMenu.getCarIndex();
		if (_GameState == GameState::Running) {
			_HeadsUpDisplay.setMaxHealth(_GameObjectContainer.getPlayerCar().getMaxHealth());
			_HeadsUpDisplay.setMaxEnergy(_GameObjectContainer.getPlayerCar().getMaxEnergy());
			_HeadsUpDisplay.setTotalLevelTime(_Level.getTotalLevelTime());
			_Clock.restart();
			_Level.resetTimer();
			setDifficulty(_OptionsMenu.getDifficulty());
			_GameObjectContainer.setLevel(_Level.getLevel());
		}
		else if (_GameState == GameState::Highscores) {
			_HighscoreMenu.loadScoreTable();
		}
		else if (_GameState == GameState::Options) {
			_OptionsMenu.enableDifficultySelection(true);
			_OptionsMenu.setReturnState(GameState::Main);
		}

		if (_CurrentCarSkinIndex < 0) {
			_CurrentCarSkinIndex = _CarSkins.size() - 1;
		}
		else if (_CurrentCarSkinIndex >= _CarSkins.size()) {
			_CurrentCarSkinIndex = 0;
		}
		_MainMenu.setCarIndex(_CurrentCarSkinIndex);
		_GameObjectContainer.getPlayerCar().setTexture((*_CarSkins.at(_CurrentCarSkinIndex)));
		_GameObjectContainer.getPlayerCar().setStats(_CurrentCarSkinIndex);
		break;
	case GameState::Highscores:
		_GameState = _HighscoreMenu.handleEvents(_RenderWindow);
		break;
	case GameState::Options:
		_GameState = _OptionsMenu.handleEvents(_RenderWindow);
		setVolume(_OptionsMenu.getVolume());
		_FPS = _OptionsMenu.getFPS();
		break;
	case GameState::About:
		_GameState = _AboutScreen.handleEvents(_RenderWindow);
		break;
	case GameState::LevelUp:
		while (_RenderWindow.pollEvent(_Event)) {
			if (_Event.type == sf::Event::Closed) {
				_GameState = GameState::Exiting;
			}
		}
		break;
	case GameState::BossFight:
		while (_RenderWindow.pollEvent(_Event))
		{
			if (_Event.type == sf::Event::Closed) {
				_GameState = GameState::Exiting;
			}
			else {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 7)) {
					_GameState = GameState::Pause;
					_PauseMenu.setReturnState(GameState::BossFight);
				}
				else {
					_GameObjectContainer.handleEvent(_Event);
				}
			}
		}
		break;
	case GameState::GameOver:
		setVolume(_OptionsMenu.getVolume());
		_GameState = _GameOverScreen.handleEvents(_RenderWindow);
		if (_GameState == GameState::Main) {
			resetGame();
		}
		break;
	case GameState::MultiplayerSelection:
		_GameState = _MultiplayerMenu.handleEvents(_RenderWindow);
		break;
	case GameState::Lobby:
		_GameState = _MultiplayerLobby.handleEvents(_RenderWindow);
		break;
	}
}

void Framework::update()
{
	switch (_GameState) {
	case GameState::Running:
		if (_Level.update(_FrameTime, _GameState)) {
			if (_GameObjectContainer.emptyScreen()) {
				_GameObjectContainer.enterBossFight();
				_GameState = GameState::BossFight;
			}
		}
		_GameObjectContainer.update(_FrameTime, _Level.getRoadSpeed());
		_HeadsUpDisplay.update(_Score, _GameObjectContainer.getPlayerCar().getHealth(), _GameObjectContainer.getPlayerCar().getEnergy(), _Level.getLevel(), _Level.getLevelTime());
		if (!_GameObjectContainer.playerIsAlive()) {
			_GameState = GameState::GameOver;
		}
		
		addScore();
		break;
	case GameState::BossFight:
		_Level.update(_FrameTime, _GameState);
		if (_GameObjectContainer.bossIsDead()) {
			_LevelUpScreen.levelUp();
			_GameState = GameState::LevelUp;
		}
		_GameObjectContainer.update(_FrameTime, _Level.getRoadSpeed());
		_HeadsUpDisplay.update(_Score, _GameObjectContainer.getPlayerCar().getHealth(), _GameObjectContainer.getPlayerCar().getEnergy(), _Level.getLevel(), _Level.getLevelTime());
		if (!_GameObjectContainer.playerIsAlive()) {
			_GameState = GameState::GameOver;
		}
		
		addScore();
		break;
	case GameState::LevelUp:
		if (_LevelUpScreen.update()) {
			_Clock.restart();
			_GameObjectContainer.getPlayerCar().resetMovement();
			_GameState = GameState::Running;
			_Level.levelUp();
			setVolume(_OptionsMenu.getVolume());
			_GameObjectContainer.setLevel(_Level.getLevel());
			_HeadsUpDisplay.setTotalLevelTime(_Level.getTotalLevelTime());
		}
		break;
	case GameState::Main:
		_Level.update(_FrameTime, _GameState);
		break;
	case GameState::GameOver:
		_GameOverScreen.update(_Score, _Level.getLevel());
		break;
	case GameState::Highscores:
		_Level.update(_FrameTime, _GameState);
		break;
	case GameState::Options:
		_OptionsMenu.update(_FrameTime);
		if (_OptionsMenu.getReturnState() == GameState::Main) {
			_Level.update(_FrameTime, _GameState);
		}
		break;
	case GameState::About:
		if (_OptionsMenu.getReturnState() == GameState::Main) {
		_Level.update(_FrameTime, _GameState);
		}
		break;
	case GameState::Loading:
		if (!_LoadingScreen.isFadingAway()) {
			load();
			_OptionsMenu.setFPS(_FPS);
			_LoadingScreen.fadeAway();
		}
		else if (_LoadingScreen.isDoneFading()) {
			_GameState = GameState::Main;
		}
		else if (_LoadingScreen.isFadingAway()) {
			_Level.update(_FrameTime, _GameState);
		}
		break;
	case GameState::Exiting:
		_IsRunning = false;
		_RenderWindow.close();
		break;
	case GameState::MultiplayerSelection:
		_MultiplayerMenu.update(_FrameTime);
		_Level.update(_FrameTime, _GameState);
		break;
	case GameState::Lobby:
		_MultiplayerLobby.update(_FrameTime);
		_Level.update(_FrameTime, _GameState);
		break;
	}
}

void Framework::playSounds() {
	if (_GameState == GameState::Running || 
		_GameState == GameState::BossFight || 
		_GameState == GameState::LevelUp || 
		_GameState == GameState::Pause || 
		(_GameState == GameState::Options && _OptionsMenu.getReturnState() == GameState::Pause))
	{
		_MenuMusic.stop();
		_Level.playMusic();
		_GameObjectContainer.playSounds();
		if (_GameState == GameState::LevelUp) {
			_LevelUpScreen.playSound();
		}
	}
	else if (_GameState == GameState::Main || _GameState == GameState::Highscores || (_GameState == GameState::Options && _OptionsMenu.getReturnState() == GameState::Pause)) {
		_Level.pauseMusic();
		if (_MenuMusic.getStatus() == sf::Sound::Stopped || _MenuMusic.getStatus() == sf::Sound::Paused) {
			_MenuMusic.play();
		}
	}
	else if (_GameState == GameState::GameOver) {
		_Level.stopMusic();
		_GameOverScreen.playSounds();
	}
}

bool Framework::measureTime()
{
	_FrameTime = _Clock.getElapsedTime().asMicroseconds() / 1000000.0f;
	_LastFPSCheck += _FrameTime;
	_LastFPSPrint += _FrameTime;
	_Clock.restart();
	
	if (_LastFPSCheck >= 1 / _FPS) {
		if (_LastFPSPrint > 1) {
			std::cout << "FPS: " << 1 / _LastFPSCheck << std::endl;
			_LastFPSPrint = 0;
		}
		_LastFPSCheck = 0;
		return true;
	}
	return false;
}

void Framework::load()
{
	try {
	if (_MenuMusicBuffer.loadFromFile("Resources/Sound/Music/menu1.ogg")) {
		_MenuMusic.setBuffer(_MenuMusicBuffer);
	}
	
	for (unsigned int i = 1; i < 7; i++) 
	{
		sf::Texture texture;
		if (texture.loadFromFile("Resources/Texture/PlayerCar/playercar" + std::to_string(i) + ".png")) 
		{
			texture.setSmooth(true);
			_CarSkins.push_back(std::make_shared<sf::Texture>(texture));
		}
	}

	_GameObjectContainer.load();
	_GameObjectContainer.setCarSkins(_CarSkins);
	_GameObjectContainer.resetGameObjects(0);

	_GameOverScreen.load();

	_OptionsMenu.loadOptions();
	_FPS = _OptionsMenu.getFPS();
	setVolume(_OptionsMenu.getVolume());
	
		_Level.load();
	_Level.resetLevel();
}
	catch (...) {
		std::exit;
	}
}

void Framework::resetGame() 
{
	_Clock.restart();
	_Score = 0;
	_GameObjectContainer.resetGameObjects(_CurrentCarSkinIndex);
	_Level.resetLevel();
}

void Framework::setVolume(float volume)
{
	sf::Listener::setGlobalVolume(volume * 7);
	_MenuMusic.setVolume(volume * 9);
	_Level.setVolume(volume * 7);
	_GameObjectContainer.setVolume(volume * 2.7);
	_LevelUpScreen.setVolume(volume * 100);
	_GameOverScreen.setVolume(volume * 10);
}

void Framework::setMouseVisible(bool visible)
{
	if (!visible) {
		int cursor = ShowCursor(0);
		while (cursor > 0) {
			cursor = ShowCursor(0);
		}
	}
	else {
		int cursor = ShowCursor(1);
		while (cursor < 0) {
			cursor = ShowCursor(1);
		}
	}
}

void Framework::setDifficulty(int Difficulty)
{
	_Level.setDifficulty(Difficulty);
	_GameObjectContainer.setDifficulty(Difficulty);
}

void Framework::addScore()
{
	_Score += _GameObjectContainer.getCarScore();
	
	switch (_OptionsMenu.getDifficulty())
	{
	case 0:
		_Score += 5 * _Level.getLevel() * _FrameTime;
		break;
	case 1:
		_Score += 10 * (int)std::powf((float)_Level.getLevel(), 1.15f) * _FrameTime;
		break;
	case 2:
		_Score += 30 * (int)std::powf((float)_Level.getLevel(), 1.3f) * _FrameTime;
		break;
	case 3:
		_Score += 60 * (int)std::powf((float)_Level.getLevel(), 1.6f) * _FrameTime;
		break;
	default:
		break;
	}
}