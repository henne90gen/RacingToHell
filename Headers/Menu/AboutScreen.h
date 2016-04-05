#pragma once
#include "Menu\Menu.h"

class AboutScreen : public Menu
{
public:
	AboutScreen();
	~AboutScreen() {}

	void render(sf::RenderWindow& window);
	GameState handleEvents(sf::RenderWindow & window);
	GameState handleMenuItemResult(MenuResult result);

private:
	sf::Text _Text2, _LevelUp, _LevelUp2, _Music, _Cars, _Boss, _Boss2, _AIShotSounds, _AIShotSounds2, _PlayerCarShotSounds, _PlayerCarShotSounds2;
	sf::RectangleShape _Frame;
};

