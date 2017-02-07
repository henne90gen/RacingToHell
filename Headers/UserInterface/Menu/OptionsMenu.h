#pragma once
#include "Menu.h"
#include "MultiplayerMenu.h"
#include "UserInterface/Menu/MenuItem/CheckBox.h"

class OptionsMenu : public Menu
{
public:
	/*
		In this menu one can change the fps limit and the volume
	*/
	OptionsMenu(Framework &framework);
	virtual ~OptionsMenu() {}

	/*
		Renders the OptionsMenu to the specified RenderWindow
		@param window Window to draw to
	*/
	virtual void render(sf::RenderWindow& Window);

	/*
		Handles events for the OptionsMenu
		@param window RenderWindow to poll the events from
		@return GameState New GameState the game should be in
	*/
	virtual void handleEvent(sf::Event &event);

	void update(float FrameTime);

private:

	int _ChangeSliderValue;

	enum Items {
		FPSIndex, VolumeIndex, DifficultyIndex, GameModeIndex, DebugIndex
	};

	sf::Text _FPS, _Volume, _ScoreMultiplierText;
	sf::RectangleShape _FPSBackground, _VolumeBackground, _ScoreMultiplierBackground;

	std::string floatToString(float value, unsigned int precision);
};

