#pragma once
#include "Menu/Menu.h"
#include "Menu/StatBox.h"

class MainMenu : public Menu
{
public:
	MainMenu();
	~MainMenu();

	void render(sf::RenderWindow& Window);
	void render(sf::RenderWindow& Window, int SelectedCar);
	GameState handleEvents(sf::RenderWindow& Window);
	GameState handleEvents(sf::RenderWindow& Window, int& carSkinIndex);

private:
	sf::Sprite _CarSkin;
	StatBox* _StatBox;
};

