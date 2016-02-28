#include "stdafx.h"
#include "Menu/OptionsMenu.h"

OptionsMenu::OptionsMenu() : _Volume(2.5f), _MaxVolume(5.0f), _MousePressed(false)
{
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 320), sf::Vector2f(150, 50), MenuResult::Back));

	_Text.setString("Main Menu");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));

	if (_Font.loadFromFile("Resources/Font/arial.ttf")) {
		_VolumeText.setFont(_Font);
		_VolumeText.setString("Volume:");
		_VolumeText.setPosition(sf::Vector2f(100,200));
	}

	_VolumeLine.setFillColor(sf::Color::Black);
	_VolumeLine.setSize(sf::Vector2f(200, 5));
	_VolumeLine.setPosition(sf::Vector2f(_VolumeText.getPosition().x + _VolumeText.getLocalBounds().width + 10, 
		_VolumeText.getPosition().y + _VolumeText.getLocalBounds().height - 1));

	_VolumeSlider.setFillColor(sf::Color::White);
	_VolumeSlider.setSize(sf::Vector2f(10,30));
	_VolumeSlider.setOrigin(sf::Vector2f(_VolumeSlider.getLocalBounds().width / 2.0f, _VolumeSlider.getLocalBounds().height / 2.0f));
	_VolumeSlider.setPosition(sf::Vector2f(_VolumeLine.getPosition().x + _VolumeLine.getLocalBounds().width * _Volume / _MaxVolume,
		_VolumeLine.getPosition().y + _VolumeLine.getLocalBounds().height / 2.0f));

	_VolumeBox.left = _VolumeLine.getPosition().x;
	_VolumeBox.top = _VolumeSlider.getPosition().y - _VolumeSlider.getLocalBounds().height / 2.0f;
	_VolumeBox.width = _VolumeLine.getLocalBounds().width;
	_VolumeBox.height = _VolumeSlider.getLocalBounds().height;
}


OptionsMenu::~OptionsMenu()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		delete _MenuItems[i];
		_MenuItems[i] = nullptr;
	}
}

void OptionsMenu::render(sf::RenderWindow & Window)
{
	Window.draw(_VolumeText);
	Window.draw(_VolumeLine);
	Window.draw(_VolumeSlider);

	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}

	Window.draw(_Text);

	if (MenuItemHovered())
	{
		sf::StandardCursor Cursor(sf::StandardCursor::HAND);
		Cursor.set(Window.getSystemHandle());
	}
	else
	{
		sf::StandardCursor Cursor(sf::StandardCursor::NORMAL);
		Cursor.set(Window.getSystemHandle());
	}
}

GameState OptionsMenu::handleEvents(sf::RenderWindow & Window)
{
	while (Window.pollEvent(_Event)) {
		sf::Vector2f MousePos = sf::Vector2f(sf::Mouse::getPosition(Window));
		if (_Event.type == sf::Event::MouseButtonPressed) {
			if (MousePos.y > _VolumeBox.top && MousePos.y < _VolumeBox.top + _VolumeBox.height && MousePos.x > _VolumeBox.left && MousePos.x < _VolumeBox.left + _VolumeBox.width) {
				_VolumeSlider.setPosition(MousePos.x, _VolumeLine.getPosition().y + _VolumeLine.getLocalBounds().height / 2.0f);
				_Volume = (_VolumeSlider.getPosition().x - _VolumeLine.getPosition().x) * _MaxVolume / _VolumeLine.getLocalBounds().width;
				_MousePressed = true;
			} else {
				for (int i = 0; i < getMenuItems().size(); i++) {
					sf::FloatRect rect = getMenuItems()[i]->getRect();
					if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) {
						switch (getMenuItems()[i]->getAction()) {
						case MenuResult::Back:
							return _ReturnState;
							break;
						case MenuResult::Nothing:
							return GameState::Options;
							break;
						case MenuResult::Exit:
							return GameState::Exiting;
							break;
						}
					}
				}
			}
		}
		else if (_Event.type == sf::Event::MouseButtonReleased) {
			_MousePressed = false;
		}
		else if (_Event.type == sf::Event::Closed) {
			return GameState::Exiting;
		}
		else if (_MousePressed) {
			if (MousePos.x < _VolumeLine.getPosition().x) {
				_VolumeSlider.setPosition(_VolumeLine.getPosition().x, _VolumeLine.getPosition().y + _VolumeLine.getLocalBounds().height / 2.0f);
			}
			else if (MousePos.x > _VolumeLine.getPosition().x + _VolumeLine.getLocalBounds().width) {
				_VolumeSlider.setPosition(_VolumeLine.getPosition().x + _VolumeLine.getLocalBounds().width, _VolumeLine.getPosition().y + _VolumeLine.getLocalBounds().height / 2.0f);
			}
			else {
				_VolumeSlider.setPosition(MousePos.x, _VolumeLine.getPosition().y + _VolumeLine.getLocalBounds().height / 2.0f);
			}
			_Volume = (_VolumeSlider.getPosition().x - _VolumeLine.getPosition().x) * _MaxVolume / _VolumeLine.getLocalBounds().width;
		}
		
	}
	return GameState::Options;
}
