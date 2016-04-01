#pragma once
#include "Menu/MenuItem/MenuButton.h"
#include "Menu/MenuItem/Slider.h"
#include "Menu/MenuItem/Textbox.h"
#include "Menu/MenuItem/MenuResults.h"
#include "Menu/MenuItem/ComboBox.h"
#include "Menu/MenuItem/HighscoreList.h"
#include "Menu/MenuItem/PlayerTable.h"
#include "StandardCursor.h"

class Menu
{
public:
	Menu(GameState menuState);
	~Menu() {}

	virtual void render(sf::RenderWindow & window) = 0;
	virtual GameState handleEvents(sf::RenderWindow & window) = 0;

	GameState handleMenuItems(sf::Event & Event);
	void handleJoystick(float axis);
	void applyJoystickSelection(sf::Event & Event);

protected:
	std::vector<std::shared_ptr<MenuItem>> _MenuItems;
	sf::Font _Font;
	sf::Text _Text;
	sf::Event _Event;
	GameState _MenuGameState;
	sf::Vector2f _MousePos;

	virtual GameState handleMenuItemResult(MenuResult result) = 0;
	bool checkMenuItemHovered(sf::RenderWindow& Window, int index);

	int _JoystickSelection;
	sf::Clock _JoystickTimer;
	float _JoystickDelay;
};

