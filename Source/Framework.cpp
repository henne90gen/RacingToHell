#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework() : _FrameTime(0), _FPS(60.0f), _IsRunning(true), _GameState(GameState::Loading)
{
	_RenderWindow.create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32U), "Racing to Hell", sf::Style::Close);
	_RenderWindow.setMouseCursorVisible(false);

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
				}
				else {
					_GameObjectContainer.handleEvent(_Event);
				}
			}
		}
		break;
	case GameState::Pause:
		_OptionsMenu.setReturnState(_GameState);
		_GameState = _PauseMenu.handleEvents(_RenderWindow);
		if (_GameState == GameState::Running) {
			_Clock.restart();
			_Level.resetTimer();
		}
		else if (_GameState == GameState::Main) {
			resetGame();
		}
		break;
	case GameState::Main:
		_OptionsMenu.setReturnState(_GameState);
		_GameState = _MainMenu.handleEvents(_RenderWindow);
		_CurrentCarSkinIndex = _MainMenu.getCarIndex();
		if (_GameState == GameState::Running) {
			_HeadsUpDisplay.setMaxHealth(_GameObjectContainer.getPlayerCar().getMaxHealth());
			_HeadsUpDisplay.setMaxEnergy(_GameObjectContainer.getPlayerCar().getMaxEnergy());
			_Clock.restart();
			_Level.resetTimer();
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
		_GameObjectContainer.update(_FrameTime, _Level.getDifficulty(), _Level.getRoadSpeed());
		_HeadsUpDisplay.update(_Score, _GameObjectContainer.getPlayerCar().getHealth(), _GameObjectContainer.getPlayerCar().getEnergy());
		if (!_GameObjectContainer.playerIsAlive()) {
			_GameState = GameState::GameOver;
		}
		_Score += _GameObjectContainer.getCarScore();
		_Score += 10 * _Level.getDifficulty() * _FrameTime;
		break;
	case GameState::BossFight:
		_Level.update(_FrameTime, _GameState);
		if (_GameObjectContainer.bossIsDead()) {
			_LevelUpScreen.levelUp();
			_GameState = GameState::LevelUp;
		}
		_GameObjectContainer.update(_FrameTime, _Level.getDifficulty(), _Level.getRoadSpeed());
		_HeadsUpDisplay.update(_Score, _GameObjectContainer.getPlayerCar().getHealth(), _GameObjectContainer.getPlayerCar().getEnergy());
		if (!_GameObjectContainer.playerIsAlive()) {
			_GameState = GameState::GameOver;
		}
		_Score += _GameObjectContainer.getCarScore();
		_Score += 100 * _Level.getDifficulty() * _FrameTime;
		break;
	case GameState::LevelUp:
		if (_LevelUpScreen.update()) {
			_Clock.restart();
			_GameObjectContainer.getPlayerCar().resetMovement();
			_GameState = GameState::Running;
			_Level.levelUp();
		}
		break;
	case GameState::Main:
		_Level.update(_FrameTime, _GameState);
		break;
	case GameState::GameOver:
		_GameOverScreen.update(_Score, _Level.getDifficulty());
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
	}
}

void Framework::playSounds() {
	if (_GameState == GameState::Running) {
		_MenuMusic.stop();
		_Level.playMusic();
		_GameObjectContainer.playSounds();
	}
	else if (_GameState == GameState::Main || _GameState == GameState::Pause || _GameState == GameState::Options) {
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
	if (_MenuMusicBuffer.loadFromFile("Resources/Sound/Music/menu1.ogg")) {
		_MenuMusic.setBuffer(_MenuMusicBuffer);
	}
	
	_Level.load();

	for (unsigned int i = 1; i < 7; i++) 
	{
		sf::Texture texture;
		if (texture.loadFromFile("Resources/Texture/PlayerCar/playercar" + std::to_string(i) + ".png")) 
		{
			_CarSkins.push_back(std::make_shared<sf::Texture>(texture));
		}
	}

	_GameObjectContainer.load();
	_GameObjectContainer.setCarSkins(_CarSkins);
	_GameObjectContainer.resetGameObjects(0);

	_GameOverScreen.load();

	//Einstellungen laden
	std::vector<std::string> Settings;
	std::string Option;
	std::ifstream FileStream;

	FileStream.open("Resources/Data/Settings.cfg");
	while (std::getline(FileStream, Option))
	{
		Settings.push_back(Option);
	}
	FileStream.close();

	if (Settings.size() == 2)
	{
		_FPS = std::stoi(Settings[0]);
		_OptionsMenu.setFPS(_FPS);
		_OptionsMenu.setVolume(std::stoi(Settings[1]));
	}
	
	setVolume(_OptionsMenu.getVolume());
}

void Framework::resetGame() 
{
	_Clock.restart();
	_Score = 0;
	_GameObjectContainer.resetGameObjects(_CurrentCarSkinIndex);
	_Level.resetLevel();
}

void Framework::setVolume(float Volume)
{
	_MenuMusic.setVolume(Volume);
	_Level.setVolume(Volume);
	_GameObjectContainer.setVolume(Volume);
	_GameOverScreen.setVolume(Volume);
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