#include "stdafx.h"
#include "Menu.h"


Menu::Menu()
{
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2 - 30, 50), MenuResult::Resume));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(10, 225), MenuResult::PreviousSkin));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(450, 225), MenuResult::NextSkin));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2 - 20, 450), MenuResult::Exit));
	}

Menu::~Menu()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		delete _MenuItems[i];
		_MenuItems[i] = nullptr;
}
}

MenuResult Menu::render(sf::RenderWindow & Window, sf::Texture carSkin)
{
	_CarSkin.setTexture(carSkin);
	_CarSkin.setPosition(sf::Vector2f(SCREENWIDTH / 2 - 20, 190));
	_CarSkin.setScale(2.5, 2.5);

	Window.clear(sf::Color::Green);
	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}
	Window.draw(_CarSkin);
	Window.display();
	return getMenuResponse(Window);
}

MenuResult Menu::getMenuResponse(sf::RenderWindow& Window) {
	sf::Event event;
	while (Window.pollEvent(event)) {
		checkMouseHover(Window);
		if (event.type == sf::Event::MouseButtonPressed) {
			return handleClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
		}
		else if (event.type == sf::Event::Closed) {
			return MenuResult::Exit;
		}
	}
}


MenuResult Menu::handleClick(sf::Vector2f MousePos)
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		sf::FloatRect rect = _MenuItems[i]->getRect();
		if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) {
			return _MenuItems[i]->getAction();
		}
	}
	return MenuResult::Nothing;
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
