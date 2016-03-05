#include "stdafx.h"
#include "Menu/OptionsMenu.h"

OptionsMenu::OptionsMenu() : _MousePressed(false)
{
	_MenuItems.push_back(new Slider(sf::Vector2f(sf::Vector2f(SCREENWIDTH / 2 - 100, 250)), MenuResult::Nothing, "Volume", 0.0f, 5.0f));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 320), sf::Vector2f(150, 50), MenuResult::Back, "Back", TextAlignment::Center));

	_JoystickSelection = 1;

	_Text.setString("Options");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
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
	Window.draw(_Text);

	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}

	checkMenuItemHovered(Window);
}

GameState OptionsMenu::handleEvents(sf::RenderWindow & Window)
{
	while (Window.pollEvent(_Event)) {
		sf::Vector2f MousePos = sf::Vector2f(sf::Mouse::getPosition(Window));
		if (_Event.type == sf::Event::Closed) {
			return GameState::Exiting;
		}
		else if (_Event.type == sf::Event::MouseButtonPressed) {
			for (int i = 0; i < _MenuItems.size(); i++) {
				sf::FloatRect rect = _MenuItems[i]->getRect();
				if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width)
				{
					if (_MenuItems[i]->getType() == MenuItems::MSlider) {
						dynamic_cast<Slider*>(_MenuItems[i])->moveSlider(MousePos);
					}
					else {
						return handleMenuItemAction(i);
					}
				}
			}
		}
		else if (_Event.type == sf::Event::JoystickButtonPressed) {
			if (sf::Joystick::isButtonPressed(0, 0)) {
				return handleMenuItemAction(_JoystickSelection);
			}
		}
		/*else if (_Event.type == sf::Event::JoystickMoved) {
			float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
			float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
			if (Y < -80 && _JoystickSelection == 0 && _JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay) {
				_MenuItems[_JoystickSelection]->switchHoverState(false, false);
				_JoystickSelection = 1;
				_JoystickTimer.restart();
			}
			else if (Y > 80 && _JoystickSelection == 1 && _JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay) {
				_JoystickSelection = 0;
				_MenuItems[_JoystickSelection]->switchHoverState(true, true);
				_JoystickTimer.restart();
			}
			else if (X < -80 && _JoystickSelection == 1) {
				if (_VolumeSlider.getPosition().x - 1 > _VolumeLine.getPosition().x) {
					_VolumeSlider.setPosition(_VolumeSlider.getPosition().x - 1, _VolumeLine.getPosition().y + _VolumeLine.getSize().y / 2.0f);
				}
			}
			else if (X > 80 && _JoystickSelection == 1) {
				if (_VolumeSlider.getPosition().x + 1 < _VolumeLine.getPosition().x + _VolumeLine.getSize().x) {
					_VolumeSlider.setPosition(_VolumeSlider.getPosition().x + 1, _VolumeLine.getPosition().y + _VolumeLine.getSize().y / 2.0f);
				}
			}
			_Volume = (_VolumeSlider.getPosition().x - _VolumeLine.getPosition().x) * _MaxVolume / _VolumeLine.getSize().x;
		}*/
		else if (_Event.type == sf::Event::MouseMoved) {
			if (_JoystickSelection == 1) {
				_MenuItems[_JoystickSelection]->switchHoverState(false, false);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				dynamic_cast<Slider*>(_MenuItems[0])->moveSlider(MousePos);
			}
		}
		else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < 10 && sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > -10) {
			_JoystickTimer.restart();
		}
	}

	/*if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -80 || sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 80) {
		float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		if (X < -80 && _JoystickSelection == 1) {
			if (_VolumeSlider.getPosition().x - 1 > _VolumeLine.getPosition().x) {
				_VolumeSlider.setPosition(_VolumeSlider.getPosition().x - 1, _VolumeLine.getPosition().y + _VolumeLine.getSize().y / 2.0f);
			}
		}
		else if (X > 80 && _JoystickSelection == 1) {
			if (_VolumeSlider.getPosition().x + 1 < _VolumeLine.getPosition().x + _VolumeLine.getSize().x) {
				_VolumeSlider.setPosition(_VolumeSlider.getPosition().x + 1, _VolumeLine.getPosition().y + _VolumeLine.getSize().y / 2.0f);
			}
		}
		_Volume = (_VolumeSlider.getPosition().x - _VolumeLine.getPosition().x) * _MaxVolume / _VolumeLine.getSize().x;
	}*/

	return GameState::Options;
}

GameState OptionsMenu::handleMenuItemAction(int index)
{
	switch (_MenuItems[index]->getAction()) {
	case MenuResult::Back:
		return _ReturnState;
		break;
	}
	return GameState::Options;
}
