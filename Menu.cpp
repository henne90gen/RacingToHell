#include "stdafx.h"
#include "Menu.h"


Menu::Menu()
{
	sf::Texture texture;
	if (texture.loadFromFile("Resources/car.png")) {
		_Background = sf::Sprite(texture);
		_Background.setPosition(sf::Vector2f(0, 0));
	}

	_MenuItems.push_back(new MenuItem(sf::Vector2f(50, 50), MenuResult::Resume));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(50, 150), MenuResult::Exit));
}

Menu::~Menu()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		delete _MenuItems[i];
	}
}

MenuResult Menu::render(sf::RenderWindow & Window)
{
	Window.clear(sf::Color::Green);
	for (int i = 0; i < _MenuItems.size(); i++) {
		std::cout << "Rendering MenuItem #" << i << std::endl;
		_MenuItems[i]->render(Window);
	}
	Window.display();
	return getMenuResponse(Window);
}

MenuResult Menu::getMenuResponse(sf::RenderWindow& Window) {
	sf::Event event;
	while (true) {
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
	Window.clear(sf::Color::Green);
	sf::Vector2i MousePos = sf::Mouse::getPosition(Window);
	for (int i = 0; i < _MenuItems.size(); i++) {
		sf::FloatRect rect = _MenuItems[i]->getRect();
		if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) {
			_MenuItems[i]->switchHoverState(true, Window);
		}
		else {
			_MenuItems[i]->switchHoverState(false, Window);
		}
	}
	Window.display();
}
