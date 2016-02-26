#include "stdafx.h"
#include "Menu/Options.h"

Options::Options()
{
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 320), MenuResult::Back));

	if (_Font.loadFromFile("Resources/Font/arial.ttf")) {
		_VolumeText.setFont(_Font);
		_VolumeText.setString("Volume:");
		_VolumeText.setPosition(sf::Vector2f(100,200));
	}
	_VolumeLine.setFillColor(sf::Color::Black);
	_VolumeLine.setPosition(sf::Vector2f(_VolumeText.getPosition().x + _VolumeText.getLocalBounds().width, _VolumeText.getPosition().y + _VolumeText.getLocalBounds().height));
	_VolumeLine.setSize(sf::Vector2f(100,5));

	_VolumeSlider.setFillColor(sf::Color::White);
	_VolumeSlider.setPosition(sf::Vector2f(_VolumeLine.getPosition().x, _VolumeLine.getPosition().y));
	_VolumeSlider.setSize(sf::Vector2f(10,30));
}


Options::~Options()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		delete _MenuItems[i];
		_MenuItems[i] = nullptr;
	}
}

void Options::render(sf::RenderWindow & Window)
{
	Window.draw(_VolumeText);
	Window.draw(_VolumeLine);
	Window.draw(_VolumeSlider);

	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}
}

void Options::checkMouseHover(sf::RenderWindow & Window)
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