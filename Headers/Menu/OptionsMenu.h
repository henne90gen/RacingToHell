#pragma once
#include "Menu/Menu.h"
#include <fstream>

class OptionsMenu : public Menu
{
public:
	/*
		In this menu one can change the fps limit and the volume
	*/
	OptionsMenu();
	~OptionsMenu() {}

	/*
		Renders the OptionsMenu to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow& Window);

	/*
		Handles events for the OptionsMenu
		@param window RenderWindow to poll the events from
		@return GameState New GameState the game should be in
	*/
	GameState handleEvents(sf::RenderWindow& Window);

	/*
		Handles the result of the different MenuItems that are on the OptionsMenu
		@param result What the MenuItems returned as a MenuResult
		@return GameState New GameState the game should be in
	*/
	GameState handleMenuItemResult(MenuResult result);

	void update(float FrameTime);

	void setReturnState(GameState returnState) { _ReturnState = returnState; }
	GameState getReturnState() { return _ReturnState; }
	float getFPS() { return _MenuItems[FPS]->getValue() + 30.0f; }
	void setFPS(float fps) { _MenuItems[FPS]->setValue(fps - 30.0f); }
	float getVolume() { return _MenuItems[Volume]->getValue(); }
	void setVolume(float volume) { _MenuItems[Volume]->setValue(volume); }
	void saveOptions();
private:
	int _ChangeSliderValue;

	enum Items {
		FPS, Volume
	};

	sf::Text _FPS;
	sf::Text _Volume;

	GameState _ReturnState;
};

