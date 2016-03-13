#pragma once
#include "Menu/Menu.h"
#include "Menu/MenuItem/StatBox.h"

class MainMenu : public Menu
{
public:
	MainMenu();
	~MainMenu();

	void render(sf::RenderWindow & Window);
	GameState handleEvents(sf::RenderWindow & Window);
	GameState handleMenuItemResult(MenuResult result);

	void setCarIndex(int CarIndex) { _SelectedCar = CarIndex; }
	int getCarIndex() { return _SelectedCar; }
private:
	sf::Sprite _CarSkin;
	StatBox* _StatBox;

	int _SelectedCar;
};

