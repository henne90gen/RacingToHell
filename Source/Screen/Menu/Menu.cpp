#include "stdafx.h"
#include "Screen/Menu/Menu.h"
#include "Framework/Framework.h"

Menu::Menu(Framework &framework, GameState menuState, GameState returnState) : GameScreen(framework),
/*_JoystickSelection(0), _JoystickDelay(0.15f), */_MenuGameState(menuState), _ReturnGameState(returnState) {
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

GameState Menu::handleMenuItems(sf::Event &event) {
    //applyJoystickSelection(event);

    if (event.type == sf::Event::MouseButtonPressed) {
        _MousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
    } else if (event.type == sf::Event::MouseMoved) {
        _MousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
    }

    if (event.type == sf::Event::Closed) {
        return GameState::Exiting;
    } else {
        GameState result = _MenuGameState;
        for (int i = 0; i < _MenuItems.size(); i++) {
            if (result == _MenuGameState) {
                result = handleMenuItemResult(_MenuItems[i]->handleEvent(event, _MousePos));
            }
        }
        return result;
    }
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
if (_MenuItems[index]->getHoverState() && _MenuItems[index]->getEnabled())
{
    if (_MenuItems[index]->getType() == MenuItemType::MButton || _MenuItems[index]->getType() == MenuItemType::MSlider || _MenuItems[index]->getType() == MenuItemType::MCombobox || _MenuItems[index]->getType() == MenuItemType::MLobbyList) {
        Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::HAND);
        return true;
    }
    else if (_MenuItems[index]->getType() == MenuItemType::MTextbox) {
        Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::TEXT);
        return true;
    }
    else {
        Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::NORMAL);
        return false;
        }
}
else {
    Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::NORMAL);
    return false;
}
#endif
}
