#pragma once
#include "Menu\MenuItem.h"

class Options
{
public:
	Options();
	~Options();

	void render(sf::RenderWindow& Window);

	void checkMouseHover(sf::RenderWindow& Window);

	std::vector<MenuItem*>& getMenuItems() { return _MenuItems; }

private:
	sf::Sprite _CarSkin;
	std::vector<MenuItem*> _MenuItems;
	sf::Font _Font;

	sf::Text _VolumeText;
	sf::RectangleShape _VolumeSlider;
	sf::RectangleShape _VolumeLine;
};

