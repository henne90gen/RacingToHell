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
	float getVolume() { return dynamic_cast<Slider*>(_MenuItems[1])->getValue(); }
	float getFPS() { return dynamic_cast<Slider*>(_MenuItems[0])->getValue() + 30.0f; }
	void setFPS(float fps) { dynamic_cast<Slider*>(_MenuItems[0])->setValue(fps-30); }
	void setVolume(float volume) { dynamic_cast<Slider*>(_MenuItems[1])->setValue(volume); }
	void saveOptions();
private:
	int _ChangeSliderValue;

	GameState _ReturnState;
};

