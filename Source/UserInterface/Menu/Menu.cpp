#include "stdafx.h"
#include "UserInterface/Menu/Menu.h"
#include "Framework/Framework.h"

Menu::Menu(Framework &framework, GameState menuState) : GameScreen(framework), _MenuGameState(menuState) {
    _Text.setFont(_FW.getOptionsManager().getFont());
}

void Menu::render(sf::RenderWindow &renderWindow) {
    bool oneIsHovering = false;
    for (int i = 0; i < _MenuItems.size(); i++) {
        if (!oneIsHovering) {
            oneIsHovering = checkMenuItemHovered(renderWindow, i);
        }
        _MenuItems[i]->render(renderWindow);
    }
}

MenuResult Menu::getMenuItemResult(sf::Event &event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        _MousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
    } else if (event.type == sf::Event::MouseMoved) {
        _MousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
    }
    for (int i = 0; i < _MenuItems.size(); i++) {
        MenuResult result = _MenuItems[i]->handleEvent(event, _MousePos);
        if (result != MenuResult::Nothing) {
            return result;
        }
    }
    return MenuResult::Nothing;
}

/*
void Menu::handleJoystick(float axis) {
	if (axis < 10 && axis > -10) {
		_JoystickTimer.restart();
	}

	if (_JoystickTimer.getElapsedTime().asSeconds() >= _JoystickDelay) {
		if (axis < -50 && _JoystickSelection > 0) {
			_JoystickSelection--;
			_JoystickTimer.restart();
		} else if (axis > 50 && _JoystickSelection < _MenuItems.size() - 3) {
			_JoystickSelection++;
			_JoystickTimer.restart();
		}
	}
}

void Menu::applyJoystickSelection(sf::Event & Event) {
	if (Event.type == sf::Event::MouseMoved
			&& Event.type == sf::Event::JoystickMoved) {
		for (int i = 0; i < _MenuItems.size(); i++) {
			if (_MenuItems[i]->getType() != MenuItemType::MTextbox) {
				_MenuItems[i]->setFocused(i == _JoystickSelection);
			}
		}
	}
}
*/

bool Menu::checkMenuItemHovered(sf::RenderWindow &Window, int index) {
#ifdef SFML_SYSTEM_WINDOWS
    sf::StandardCursor Cursor;
    if (_MenuItems[index]->getHoverState() && _MenuItems[index]->getEnabled()) {
        if (_MenuItems[index]->getType() == MenuItemType::MButton ||
            _MenuItems[index]->getType() == MenuItemType::MSlider ||
            _MenuItems[index]->getType() == MenuItemType::MCombobox ||
            _MenuItems[index]->getType() == MenuItemType::MLobbyList) {
            Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::HAND);
            return true;
        } else if (_MenuItems[index]->getType() == MenuItemType::MTextbox) {
            Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::TEXT);
            return true;
        } else {
            Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::NORMAL);
            return false;
        }
    } else {
        Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::NORMAL);
        return false;
    }
#endif

#ifdef SFML_SYSTEM_LINUX
    return false;
#endif
}
