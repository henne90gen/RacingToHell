#pragma once
#include "Menu/MenuItem/MenuButton.h"
#include "Menu/MenuItem/Slider.h"
#include "Menu/MenuItem/MenuResults.h"
#include "Menu/MenuItem/ComboBox.h"
#include "StandardCursor.h"

class Menu
{
public:
	Menu(GameState menuState);
	~Menu() {}

	virtual void render(sf::RenderWindow & Window) = 0;
	virtual GameState handleEvents(sf::RenderWindow & Window) = 0;

	GameState handleMenuItems(sf::Event & Event);
	void handleJoystick(float axis);
	void applyJoystickSelection();

protected:
	std::vector<std::shared_ptr<MenuItem>> _MenuItems;
	sf::Font _Font;
	sf::Text _Text;
	sf::Event _Event;
	GameState _MenuGameState;
	sf::Vector2f _MousePos;

	virtual GameState handleMenuItemResult(MenuResult result) = 0;
	void checkMenuItemHovered(sf::RenderWindow& Window);

	int _JoystickSelection;
	sf::Clock _JoystickTimer;
	float _JoystickDelay;
};

