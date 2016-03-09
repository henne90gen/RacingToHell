#pragma once
#include "Menu/MenuButton.h"
#include "Menu/Slider.h"
#include "Menu/MenuResults.h"
#include "StandardCursor.h"

class Menu
{
public:
	Menu(GameState menuState);
	~Menu();

	virtual void render(sf::RenderWindow & Window) = 0;
	virtual GameState handleEvents(sf::RenderWindow & Window) = 0;
	virtual GameState handleMenuItemResult(MenuResult result) = 0;

	GameState handleMenuItems(sf::Event & Event);
	void applyJoystickSelection();

protected:
	std::vector<MenuItem*> _MenuItems;
	sf::Font _Font;
	sf::Text _Text;
	sf::Event _Event;
	GameState _MenuGameState;
	sf::Vector2f _MousePos;

	void checkMenuItemHovered(sf::RenderWindow& Window);

	int _JoystickSelection;
	sf::Clock _JoystickTimer;
	float _JoystickDelay;
};

