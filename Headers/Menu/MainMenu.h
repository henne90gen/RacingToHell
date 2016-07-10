#pragma once
#include "Menu/Menu.h"
#include "Menu/MenuItem/StatBox.h"

class MainMenu : public Menu
{
public:
	/*
		The main menu in the game. It has buttons to start playing, view the highscores, go to options or exit.
	*/
	MainMenu();
	~MainMenu() {}

	/*
		Renders the MainMenu to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow & window);

	/*
		Handles events for the MainMenu
		@param window RenderWindow to poll the events from
		@return GameState New GameState the game should be in
	*/
	GameState handleEvents(sf::RenderWindow & window);

	/*
		Handles the result of the different MenuItems that are on the MainMenu
		@param result What the MenuItems returned as a MenuResult
		@return GameState New GameState the game should be in
	*/
	GameState handleMenuItemResult(MenuResult result);

	/*
		@param carIndex Index of the selected car
	*/
	void setCarIndex(int carIndex) { _SelectedCar = carIndex; }
	
	/*
		@return int Index of the selected car
	*/
	int getCarIndex() { return _SelectedCar; }
private:
	std::shared_ptr<StatBox> _StatBox;

	int _SelectedCar;
};

