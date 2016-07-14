#pragma once
#include "Menu/Menu.h"
#include "Menu/MultiplayerMenu.h"

class OptionsMenu : public Menu
{
public:
	/*
		In this menu one can change the fps limit and the volume
	*/
	OptionsMenu();
	virtual ~OptionsMenu() {}

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

	// TODO: Multi
	void loadOptions(/*MultiplayerMenu& mpMenu*/);
	void saveOptions(/*MultiplayerMenu& mpMenu*/);

	GameState getReturnState() { return _ReturnState; }
	void setReturnState(GameState returnState) { _ReturnState = returnState; }

	float getFPS() { return _MenuItems[FPS]->getValue() + 30.0f; }
	void setFPS(float fps) { _MenuItems[FPS]->setValue(fps - 30.0f); }

	float getVolume() { return _MenuItems[Volume]->getValue(); }
	void setVolume(float volume) { _MenuItems[Volume]->setValue(volume); }

	int getDifficulty() { return (int)_MenuItems[Difficulty]->getValue(); } 
	void setDifficulty(int Dif) { _MenuItems[Difficulty]->setValue(Dif); }
	void enableDifficultySelection(bool enabled) { _MenuItems[Difficulty]->setEnabled(enabled); }

	GameMode getGameMode() { return (GameMode)(int)_MenuItems[Mode]->getValue(); }
	void setGameMode(GameMode gm) { _MenuItems[Mode]->setValue((float)gm); }
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

