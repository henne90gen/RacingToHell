#pragma once
#include "Menu/Menu.h"
#include <fstream>

class OptionsMenu : public Menu
{
public:
	OptionsMenu();
	~OptionsMenu();

	void render(sf::RenderWindow& Window);
	GameState handleEvents(sf::RenderWindow& Window);
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

