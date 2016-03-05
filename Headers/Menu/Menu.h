#pragma once
#include "Menu/MenuButton.h"
#include "Menu/Slider.h"
#include "Menu/MenuResults.h"
#include "StandardCursor.h"

class Menu
{
public:
	Menu();
	~Menu();

	virtual void render(sf::RenderWindow& Window) = 0;
	virtual GameState handleEvents(sf::RenderWindow& Window) = 0;
	virtual GameState handleMenuItemAction(int index) = 0;

protected:
	std::vector<MenuItem*> _MenuItems;
	sf::Font _Font;
	sf::Text _Text;
	sf::Event _Event;

	void checkMenuItemHovered(sf::RenderWindow& Window);

	int _JoystickSelection;
	sf::Clock _JoystickTimer;
	float _JoystickDelay;
};

