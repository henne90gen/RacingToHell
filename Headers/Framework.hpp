#pragma once

#include "stdafx.h"
#include "GameObject\Car.h"
#include "GameObject\PlayerCar.h"
#include "GameObject\GameObjectContainer.h"
#include "Menu\Menu.h"
#include "Menu\Options.h"
#include "Menu\GameOverScreen.h"
#include "HeadsUpDisplay.h"
#include "Level\Level.h"

class Framework
{
public:
	//Constructor, Destructor
	Framework();
	~Framework();

	//Functions
	void run();

	enum GameState {
		Running, MainMenu, Pausing, OptionsMenu, GameOver, Exiting
	};

private:
	//SFML-Graphics
	sf::RenderWindow _RenderWindow;
	sf::Event _Event;
	sf::Clock _Clock;

	//Variables
	float _FrameTime;
	float _LastFPSPrint;
	bool _IsRunning;
	GameObjectContainer _GameObjectContainer;
	GameState _GameState;
	std::vector<sf::Texture*> _CarSkins;
	int _CurrentCarSkinIndex;
	Menu _Menu;
	Options _OptionsMenu;
	GameOverScreen _GameOverScreen;
	HeadsUpDisplay _HeadsUpDisplay;
	Level _Level;
	float _Score;

	//Functions
	void update(float Frametime);
	void handleEvent();
	void handleEventMenu();
	void handleEventGameOver();
	void handleEventOptions();
	void render();
	void playSounds();
	void measureTime();
	void loadCarSkins();
	void resetGame();
};