#pragma once
#include "Menu.h"
#include "MultiplayerMenu.h"

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

	/*
		Handles the result of the different MenuItems that are on the OptionsMenu
		@param result What the MenuItems returned as a MenuResult
		@return GameState New GameState the game should be in
	*/
	GameState handleMenuItemResult(MenuResult result);

	void update(float FrameTime);

	GameState getReturnState() { return _ReturnState; }
	void setReturnState(GameState returnState) { _ReturnState = returnState; }

	void enableDifficultySelection(bool enabled) { _MenuItems[Difficulty]->setEnabled(enabled); }

	void enableGameModeSelection(bool enabled) { _MenuItems[Mode]->setEnabled(enabled); }

	std::vector<float> getMultiplierList() { return _ScoreMultiplierList; }
private:
	std::vector<float> _ScoreMultiplierList;

	int _ChangeSliderValue;

	enum Items {
		FPS, Volume, Difficulty, Mode
	};

	sf::Text _FPS, _Volume, _ScoreMultiplierText;
	sf::RectangleShape _FPSBackground, _VolumeBackground, _ScoreMultiplierBackground ;

	GameState _ReturnState;

	std::string floatToString(float value, unsigned int precision);
};

