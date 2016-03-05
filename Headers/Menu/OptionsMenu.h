#pragma once
#include "Menu/Menu.h"

class OptionsMenu : public Menu
{
public:
	OptionsMenu();
	~OptionsMenu();

	void render(sf::RenderWindow& Window);
	GameState handleEvents(sf::RenderWindow& Window);
	GameState handleMenuItemAction(int index);

	void setReturnState(GameState returnState) { _ReturnState = returnState; }
	GameState getReturnState() { return _ReturnState; }
	float getVolume() { return dynamic_cast<Slider*>(_MenuItems[0])->getValue(); }

private:
	bool _MousePressed;

	GameState _ReturnState;
};

