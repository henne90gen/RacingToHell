#include "stdafx.h"
#include "Menu.h"


Menu::Menu()
{
	sf::Texture texture;
	if (texture.loadFromFile("Resources/car.png")) {
		_Background = sf::Sprite(texture);
		_Background.setPosition(sf::Vector2f(0, 0));
	}

	_MenuItems.push_back(new MenuItem(sf::Vector2f(0, 0), MenuResult::Resume));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(0, 100), MenuResult::Exit));
}

Menu::~Menu()
{
}

MenuResult Menu::render(sf::RenderWindow & Window)
{
	//Window.draw(_Background);
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
