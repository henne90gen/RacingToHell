#include "stdafx.h"
#include "Menu.h"


Menu::Menu()
{
	_MenuItems.push_back(new MenuItem(sf::Vector2f(50, 25), MenuResult::Resume));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2 - 100, SCREENHEIGHT - 75), MenuResult::PreviousSkin));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2 + 37, SCREENHEIGHT - 75), MenuResult::NextSkin));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH - 100, 25), MenuResult::Exit));
}

Menu::~Menu()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		delete _MenuItems[i];
		_MenuItems[i] = nullptr;
}
}

void Menu::render(sf::RenderWindow & Window)
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}
	Window.draw(_CarSkin);
}

void Menu::update(float FrameTime)
{
}

void Menu::checkMouseHover(sf::RenderWindow & Window)
{
	sf::Vector2i MousePos = sf::Mouse::getPosition(Window);
	for (int i = 0; i < _MenuItems.size(); i++) {
		sf::FloatRect rect = _MenuItems[i]->getRect();
		if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) {
			_MenuItems[i]->switchHoverState(true);
		}
		else {
			_MenuItems[i]->switchHoverState(false);
		}
	}
}
