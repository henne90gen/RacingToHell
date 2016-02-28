#pragma once
#include "Menu\Menu.h"

class OptionsMenu : public Menu
{
public:
	OptionsMenu();
	~OptionsMenu();

	void render(sf::RenderWindow& Window);
	GameState handleEvents(sf::RenderWindow& Window);

	void setReturnState(GameState returnState) { _ReturnState = returnState; }
	GameState getReturnState() { return _ReturnState; }
	float getVolume() { return _Volume; }

private:
	sf::Text _VolumeText;
	sf::RectangleShape _VolumeSlider;
	sf::RectangleShape _VolumeLine;
	sf::FloatRect _VolumeBox;

	bool _MousePressed;
	GameState _ReturnState;

	float _Volume, _MaxVolume;
};

