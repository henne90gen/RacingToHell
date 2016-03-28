#pragma once
#include "Menu/Menu.h"

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

	void loadOptions();
	void saveOptions();

	void setReturnState(GameState returnState) { _ReturnState = returnState; }
	GameState getReturnState() { return _ReturnState; }
	float getFPS() { return _MenuItems[FPS]->getValue() + 30.0f; }
	void setFPS(float fps) { _MenuItems[FPS]->setValue(fps - 30.0f); }
	float getVolume() { return _MenuItems[Volume]->getValue(); }
	void setVolume(float volume) { _MenuItems[Volume]->setValue(volume); }
	
	int getDifficulty() { return (int)_MenuItems[Difficulty]->getValue(); }
	void setDifficulty(int Dif) { _MenuItems[Difficulty]->setValue(Dif); }
	void enableDifficultySelection(bool enabled) { _MenuItems[Difficulty]->setEnabled(enabled); }
private:
	int _ChangeSliderValue;

	enum Items {
		FPS, Volume, Difficulty
	};

	sf::Text _FPS, _Volume;
	sf::RectangleShape _FPSBackground, _VolumeBackground;

	GameState _ReturnState;
};

