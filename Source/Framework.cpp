#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework() : _FrameTime(0), _FPS(60.0f), _IsRunning(true), _GameState(GameState::Loading), _GameMode(GameMode::Standard), _LastFPSCheck(), _LastFPSPrint(), _CurrentCarSkinIndex(), _Score()
{
	_RenderWindow.create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32U), "Racing to Hell", sf::Style::Close);
	#ifdef SFML_SYSTEM_WINDOWS
	_RenderWindow.setMouseCursorVisible(false);
    #else
	_RenderWindow.setMouseCursorVisible(true);
    #endif

	sf::Image Icon;
	if (Icon.loadFromFile("Resources/Texture/Icon/Icon.png"))
	{
		_RenderWindow.setIcon(Icon.getSize().x, Icon.getSize().y, Icon.getPixelsPtr());
	}

	//Seed
	srand(time(NULL));

	//Multiplayer
	_MultiplayerMenu.setNetworkHandle(&_NetworkHandle);
	_MultiplayerMenu.setPlayerName("Name");
	_MultiplayerLobby.setNetworkHandle(&_NetworkHandle);
}

Framework::~Framework()
{
	_CarSkins.clear();
}

void Framework::run()
{
	while (_IsRunning || _NetworkHandle.getState() != NetworkState::NoNetState)
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
	if ((_GameState != GameState::Loading || _LoadingScreen.isFadingAway()) && _GameState != GameState::Countdown) {
		_Level.render(_RenderWindow);
		if (_NetworkHandle.getRelation() == NetworkRelation::NoRel) {
			_GameObjectContainer.render(_RenderWindow, _GameState == GameState::Running || _GameState == GameState::BossFight);
		}
		else {

			_MPGameObjectContainer.render(_RenderWindow, _GameState == GameState::RunningMultiplayer || _GameState == GameState::BossFight);
		}
	}

	switch (_GameState) {
	case GameState::Running:
		setMouseVisible(false);

		#ifdef SFML_SYSTEM_LINUX
			_RenderWindow.setMouseCursorVisible(false);
        #endif

		_HeadsUpDisplay.render(_RenderWindow);
		break;
	case GameState::Main:
		setMouseVisible(true);

		#ifdef SFML_SYSTEM_LINUX
			_RenderWindow.setMouseCursorVisible(true);
		#endif

		_MainMenu.render(_RenderWindow);
		break;
	case GameState::Pause:
		setMouseVisible(true);

		#ifdef SFML_SYSTEM_LINUX
			_RenderWindow.setMouseCursorVisible(true);
		#endif

		_PauseMenu.render(_RenderWindow);
		break;
	case GameState::Highscores:
		setMouseVisible(true);

		#ifdef SFML_SYSTEM_LINUX
			_RenderWindow.setMouseCursorVisible(true);
		#endif

		_HighscoreMenu.render(_RenderWindow);
		break;
	case GameState::Options:
		setMouseVisible(true);

		#ifdef SFML_SYSTEM_LINUX
			_RenderWindow.setMouseCursorVisible(true);
		#endif

		_OptionsMenu.render(_RenderWindow);
		break;
	case GameState::About:
		setMouseVisible(true);

		#ifdef SFML_SYSTEM_LINUX
			_RenderWindow.setMouseCursorVisible(true);
		#endif

		_AboutScreen.render(_RenderWindow);
		break;
	case GameState::LevelUp:
		setMouseVisible(false);

		#ifdef SFML_SYSTEM_LINUX
			_RenderWindow.setMouseCursorVisible(false);
		#endif

		_LevelUpScreen.render(_RenderWindow);
		break;
	case GameState::BossFight:
		setMouseVisible(false);

		#ifdef SFML_SYSTEM_LINUX
			_RenderWindow.setMouseCursorVisible(false);
		#endif

		_HeadsUpDisplay.render(_RenderWindow);
		break;
	case GameState::GameOver:
		setMouseVisible(true);

		#ifdef SFML_SYSTEM_LINUX
			_RenderWindow.setMouseCursorVisible(true);
		#endif

		_GameOverScreen.render(_RenderWindow);
		break;
	case GameState::Loading:
		setMouseVisible(false);

		#ifdef SFML_SYSTEM_LINUX
			_RenderWindow.setMouseCursorVisible(false);
		#endif

		if (_LoadingScreen.isFadingAway()) {
			_MainMenu.render(_RenderWindow);
		}
		_LoadingScreen.render(_RenderWindow);
		break;
	case GameState::MultiplayerSelection:
		setMouseVisible(true);
		_MultiplayerMenu.render(_RenderWindow);
		break;
	case GameState::Connecting:
		setMouseVisible(true);
		_MultiplayerMenu.render(_RenderWindow);
		break;
	case GameState::Lobby:
		setMouseVisible(true);
		_MultiplayerLobby.render(_RenderWindow);
		break;
	case GameState::Countdown:
		setMouseVisible(false);
		_Countdown.render(_RenderWindow);
		break;
	case GameState::RunningMultiplayer:
		setMouseVisible(false);
		_HeadsUpDisplay.render(_RenderWindow);
		break;
	case GameState::PauseMultiplayer:
		setMouseVisible(true);
		_PauseMultiplayerMenu.render(_RenderWindow);
		break;
	}
	_RenderWindow.display();
}

/*









*/

void Framework::handleEvents()
{
	switch (_GameState) {
	case GameState::Running:
		if (_RenderWindow.pollEvent(_Event))
		{
			if (_Event.type == sf::Event::Closed) {
				_GameState = GameState::Exiting;
			}
			else if (_Event.type == sf::Event::MouseLeft || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 7)) {
				_PauseMenu.setReturnState(_GameState);
				_GameState = GameState::Pause;
			}
			else {
				_GameObjectContainer.handleEvent(_Event);
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
			_OptionsMenu.enableGameModeSelection(false);
			_OptionsMenu.setReturnState(GameState::Pause);
		}
		break;
	case GameState::Main:
		_GameState = _MainMenu.handleEvents(_RenderWindow);
		
		_CurrentCarSkinIndex = _MainMenu.getCarIndex();
		if (_CurrentCarSkinIndex < 0) {
			_CurrentCarSkinIndex = _CarSkins.size() - 1;
		}
		else if (_CurrentCarSkinIndex >= _CarSkins.size()) {
			_CurrentCarSkinIndex = 0;
		}
		_MainMenu.setCarIndex(_CurrentCarSkinIndex);
		_GameObjectContainer.getPlayerCar()->setTexture((*_CarSkins.at(_CurrentCarSkinIndex)));
		_GameObjectContainer.getPlayerCar()->setStats(_CurrentCarSkinIndex);
		
		_GameMode = _OptionsMenu.getGameMode();

		if (_GameMode == GameMode::Hardcore)
		{
			_GameObjectContainer.getPlayerCar()->setMaxHealth(15);
			_GameObjectContainer.getPlayerCar()->setHealth(15);
		}

		if (_GameState == GameState::Running) {
			_HeadsUpDisplay.setMaxHealth(_GameObjectContainer.getPlayerCar()->getMaxHealth());
			_HeadsUpDisplay.setMaxEnergy(_GameObjectContainer.getPlayerCar()->getMaxEnergy());
			_HeadsUpDisplay.setTotalLevelTime(_Level.getTotalLevelTime());
			_Clock.restart();
			_Level.resetTimer();
			setDifficulty(_OptionsMenu.getDifficulty());
			
			_GameObjectContainer.setGameMode(_GameMode);
			_GameObjectContainer.setLevel(_Level.getLevel());
		}
		else if (_GameState == GameState::Highscores) {
			_HighscoreMenu.loadScoreTable();
		}
		else if (_GameState == GameState::Options) {
			_OptionsMenu.enableDifficultySelection(true);
			_OptionsMenu.enableGameModeSelection(true);
			_OptionsMenu.setReturnState(GameState::Main);
		}
		else if (_GameState == GameState::MultiplayerSelection)
		{
			_MultiplayerMenu.resetFeedback();
		}
		break;
	case GameState::Highscores:
		_GameState = _HighscoreMenu.handleEvents(_RenderWindow);
		break;
	case GameState::Options:
		_GameState = _OptionsMenu.handleEvents(_RenderWindow);
		if (_GameState != GameState::Options) {
			_OptionsMenu.saveOptions(_MultiplayerMenu);
		}
		setVolume(_OptionsMenu.getVolume());
		_FPS = _OptionsMenu.getFPS();
		break;
	case GameState::About:
		_GameState = _AboutScreen.handleEvents(_RenderWindow);
		break;
	case GameState::Loading:
		while (_RenderWindow.pollEvent(_Event)) {
			if (_Event.type == sf::Event::Closed) {
				_GameState = GameState::Exiting;
			}
		}
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
		if (_GameState != GameState::MultiplayerSelection) {
			_OptionsMenu.saveOptions(_MultiplayerMenu);
		}
		if (_GameState == GameState::Lobby)
		{
			_NetworkHandle.setState(NetworkState::Lobby);
			_NetworkHandle.setRelation(NetworkRelation::Host);
			_MPGameObjectContainer.setNetworkRelation(NetworkRelation::Host);

			GameObjectFactory::setDeltaID(0);

			_MultiplayerLobby.EnableButtons(true);
			_MultiplayerLobby.setAdminTable(true);
			_MultiplayerLobby.resetRdyButton();
			_MultiplayerLobby.resetTable();
			_MultiplayerLobby.addPlayer(_NetworkHandle.getMyName(), true);

			_NetworkThread = std::thread(&NetworkHandle::run, &_NetworkHandle);
			_NetworkThread.detach();
		}

		break;
	case GameState::Connecting:
		_GameState = _MultiplayerMenu.handleEvents(_RenderWindow);
		break;
	case GameState::Lobby:
		if (_MultiplayerMenu.getCreatedLobby() == 0 && _NetworkHandle.getState() == NetworkState::NoNetState) {
			_NetworkHandle.setState(NetworkState::Lobby);
			_NetworkHandle.setRelation(NetworkRelation::Client);
			_MPGameObjectContainer.setNetworkRelation(NetworkRelation::Client);

			GameObjectFactory::setDeltaID((sf::Uint64)std::pow(2, 16) * (sf::Uint64)std::pow(2, 16) / 2);

			_MultiplayerLobby.EnableButtons(false);
			_MultiplayerLobby.setAdminTable(false);
			_MultiplayerLobby.resetTable();
			_MultiplayerLobby.addPlayer(_NetworkHandle.getMemberName(), true);
			_MultiplayerLobby.addPlayer(_NetworkHandle.getMyName(), false);

			_NetworkThread = std::thread(&NetworkHandle::run, &_NetworkHandle);
			_NetworkThread.detach();
		}

		_GameState = _MultiplayerLobby.handleEvents(_RenderWindow);
		_CurrentCarSkinIndex = _MultiplayerLobby.getCarIndex();

		if (_CurrentCarSkinIndex < 0) {
			_CurrentCarSkinIndex = _CarSkins.size() - 1;
		}
		else if (_CurrentCarSkinIndex >= _CarSkins.size()) {
			_CurrentCarSkinIndex = 0;
		}
		_MultiplayerLobby.setCarIndex(_CurrentCarSkinIndex);
		_MPGameObjectContainer.getPlayerCar().setTexture((*_CarSkins.at(_CurrentCarSkinIndex)));
		_MPGameObjectContainer.getPlayerCar().setStats(_CurrentCarSkinIndex);

		if (_GameState == GameState::Countdown) {
			_HeadsUpDisplay.setMaxHealth(_MPGameObjectContainer.getPlayerCar().getMaxHealth());
			_HeadsUpDisplay.setMaxEnergy(_MPGameObjectContainer.getPlayerCar().getMaxEnergy());
			_HeadsUpDisplay.setTotalLevelTime(_Level.getTotalLevelTime());
			_Clock.restart();
			_Level.resetTimer();
			setDifficulty(_OptionsMenu.getDifficulty());
			_GameMode = _OptionsMenu.getGameMode();
			_GameObjectContainer.setGameMode(_GameMode);
			_MPGameObjectContainer.setLevel(_Level.getLevel());

			sf::Packet packet;
			_MPGameObjectContainer.getPlayerCar() >> packet;
			_NetworkHandle.addPacket(NetworkCommunication::CreateGameObject, packet);
		}
		break;
	case GameState::Countdown:
		_GameState = _Countdown.handleEvents(_RenderWindow);
		break;
	case GameState::RunningMultiplayer:
		if (_RenderWindow.pollEvent(_Event))
		{
			if (_Event.type == sf::Event::Closed) {
				_GameState = GameState::Exiting;
			}
			else if (_Event.type == sf::Event::MouseLeft) {
				_PauseMultiplayerMenu.setReturnState(_GameState);
				_GameState = GameState::PauseMultiplayer;
			}
			else {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 7)) {
					_PauseMultiplayerMenu.setReturnState(_GameState);
					_GameState = GameState::PauseMultiplayer;
				}
				else {
					_MPGameObjectContainer.handleEvent(_Event);
				}
			}
		}
		break;
	case GameState::PauseMultiplayer:
		_MPGameObjectContainer.stopSounds();
		_GameState = _PauseMultiplayerMenu.handleEvents(_RenderWindow);
		if (_GameState == GameState::RunningMultiplayer) {
			_Clock.restart();
		}
		else if (_GameState == GameState::Main) {
			resetGame();
		}
		else if (_GameState == GameState::Options) {
			_OptionsMenu.enableDifficultySelection(false);
			_OptionsMenu.enableGameModeSelection(false);
			_OptionsMenu.setReturnState(GameState::PauseMultiplayer);
		}

		break;
	}
}

/*









*/

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
		_HeadsUpDisplay.update(_Score, _GameObjectContainer.getPlayerCar()->getHealth(), _GameObjectContainer.getPlayerCar()->getEnergy(), _Level.getLevel(), _Level.getLevelTime(), _GameMode);
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
		_HeadsUpDisplay.update(_Score, _GameObjectContainer.getPlayerCar()->getHealth(), _GameObjectContainer.getPlayerCar()->getEnergy(), _Level.getLevel(), _Level.getLevelTime(), _GameMode);
		if (!_GameObjectContainer.playerIsAlive()) {
			_GameState = GameState::GameOver;
		}
		
		addScore();
		break;
	case GameState::LevelUp:
		if (_LevelUpScreen.update()) {
			_Clock.restart();
			_GameObjectContainer.getPlayerCar()->resetMovement();
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
		_NetworkHandle.setRelation(NetworkRelation::NoRel);
		_NetworkHandle.setState(NetworkState::NoNetState);
		_RenderWindow.close();
		_MenuMusic.stop();
		_Level.stopMusic();

		break;
	case GameState::MultiplayerSelection:
		_Level.update(_FrameTime, _GameState);
		break;
	case GameState::Connecting:
	{
		_Level.update(_FrameTime, _GameState);
		NetworkCommunication netComm = _MultiplayerMenu.update(_FrameTime);
		if (netComm == NetworkCommunication::ConnectionSuccesfull) {
			_GameState = GameState::Lobby;		
		}
		else if (netComm == NetworkCommunication::ConnectionFailed || netComm == NetworkCommunication::WrongPassword) {
			_GameState = GameState::MultiplayerSelection;
		}
		break;
	}
	case GameState::Lobby:
	{
		std::pair<NetworkCommunication, int> lastResponse = _NetworkHandle.getLastResponse();

		if (lastResponse.first == NetworkCommunication::Kick)
		{
			_GameState = GameState::MultiplayerSelection;
			_MultiplayerMenu.resetFeedback();
			_MultiplayerMenu.setKickMessage();
			//_MultiplayerMenu.resetTextbox();
		}
		else if (lastResponse.first == NetworkCommunication::Disconnect && lastResponse.second == 0)
		{
			_GameState = GameState::MultiplayerSelection;
			_MultiplayerMenu.resetFeedback();
			_MultiplayerMenu.setLobbyClosedMessage();
		}
		else if (lastResponse.first == NetworkCommunication::Disconnect && lastResponse.second == 1)
		{
			_MultiplayerLobby.removePlayer(1);
		}
		else if (lastResponse.first == NetworkCommunication::ConnectionSuccesfull && _NetworkHandle.getRelation() == NetworkRelation::Host)
		{
			_MultiplayerLobby.addPlayer(_NetworkHandle.getMemberName(), false);
		}
		else if (lastResponse.first == NetworkCommunication::Ready && _NetworkHandle.getRelation() == NetworkRelation::Host)
		{
			_MultiplayerLobby.setClientReady((bool)(lastResponse.second));
		}
		else if (lastResponse.first == NetworkCommunication::StartGame && _NetworkHandle.getRelation() == NetworkRelation::Client) {
			_Countdown.fastForward((float)lastResponse.second / (float)_NetworkHandle.getTickRate());
			_HeadsUpDisplay.setMaxHealth(_MPGameObjectContainer.getPlayerCar().getMaxHealth());
			_HeadsUpDisplay.setMaxEnergy(_MPGameObjectContainer.getPlayerCar().getMaxEnergy());
			_HeadsUpDisplay.setTotalLevelTime(_Level.getTotalLevelTime());
			_Clock.restart();
			_Level.resetTimer();
			setDifficulty(_OptionsMenu.getDifficulty());
			_GameMode = _OptionsMenu.getGameMode();
			_GameObjectContainer.setGameMode(_GameMode);
			_MPGameObjectContainer.setLevel(_Level.getLevel());
			_GameState = GameState::Countdown;
		}
		
		/*if (lastResponse.first == NetworkCommunication::StartGame) {
			sf::Packet packet;
			_MPGameObjectContainer.getPlayerCar() >> packet;
			_NetworkHandle.addPacket(NetworkCommunication::CreateGameObject, packet);
		}*/

		_Level.update(_FrameTime, _GameState);
		break;
	}
	case GameState::Countdown:
		if (_Countdown.update(_FrameTime)) {
			_GameState = GameState::RunningMultiplayer;
		}
		break;
	case GameState::RunningMultiplayer:
	case GameState::PauseMultiplayer:
		//_MPGameObjectContainer.handleOutgoingPackets(_NetworkHandle.getSendPackets());
		//_MPGameObjectContainer.handleIncomingPackets(_NetworkHandle);
		if (_Level.update(_FrameTime, _GameState)) {
			if (_MPGameObjectContainer.emptyScreen()) {
				_MPGameObjectContainer.enterBossFight();
				_GameState = GameState::BossFightMultiplayer;
			}
		}
		_MPGameObjectContainer.update(_FrameTime, _Level.getRoadSpeed(), _NetworkHandle);
		_HeadsUpDisplay.update(_Score, _MPGameObjectContainer.getPlayerCar().getHealth(), _MPGameObjectContainer.getPlayerCar().getEnergy(), _Level.getLevel(), _Level.getLevelTime(), _GameMode);
		if (!_MPGameObjectContainer.playerIsAlive()) {
			_GameState = GameState::GameOverMultiplayer;
		}
		addScore();
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
	} else if (_GameState == GameState::Exiting) {
		_Level.stopMusic();
		_MenuMusic.stop();
	}
}

bool Framework::measureTime()
{
	_FrameTime = _Clock.restart().asMicroseconds() / 1000000.0f;
	_LastFPSCheck += _FrameTime;
	_LastFPSPrint += _FrameTime;
	
	if (_LastFPSCheck >= 1 / _FPS) {
		if (_LastFPSPrint > 1) {
			//std::cout << "FPS: " << 1 / _LastFPSCheck << std::endl;
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
		if (!_MenuMusic.openFromFile("Resources/Sound/Music/menu1.ogg")) {
			std::cout << "Couldn't load music" << std::endl;
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


		_MPGameObjectContainer.load();
		_MPGameObjectContainer.setCarSkins(_CarSkins);
		_MPGameObjectContainer.resetGameObjects(0);

		_GameOverScreen.load();

		_OptionsMenu.loadOptions(_MultiplayerMenu);
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
	#ifdef SFML_SYSTEM_WINDOWS
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
	#endif
}

void Framework::setDifficulty(int Difficulty)
{
	_Level.setDifficulty(Difficulty);
	_GameObjectContainer.setDifficulty(Difficulty);
}

void Framework::setGameMode(GameMode gameMode)
{

}

void Framework::addScore()
{
	float Multiplier = _OptionsMenu.getMultiplierList()[(int)_GameMode];

	_Score += _GameObjectContainer.getCarScore() * Multiplier;
	
	switch (_OptionsMenu.getDifficulty())
	{
	case 0:
		_Score += 5 * _Level.getLevel() * Multiplier * _FrameTime;
		break;
	case 1:
		_Score += 10 * (int)std::pow((float)_Level.getLevel(), 1.15f) * Multiplier * _FrameTime;
		break;
	case 2:
		_Score += 30 * (int)std::pow((float)_Level.getLevel(), 1.3f) * Multiplier * _FrameTime;
		break;
	case 3:
		_Score += 60 * (int)std::pow((float)_Level.getLevel(), 1.6f) * Multiplier * _FrameTime;
		break;
	default:
		break;
	}
}
