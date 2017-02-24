#include "stdafx.h"
#include "UserInterface/Menu/Menu.h"
#include "Framework/Framework.h"

Menu::Menu(Framework &framework, GameState menuState) : GameScreen(framework), _MenuGameState(menuState) {
    _Text.setFont(_FW.getOptionsManager().getFont());
}

bool Menu::checkMenuItemHovered(sf::RenderWindow &Window, int index) {
#ifdef SFML_SYSTEM_WINDOWS
    //    sf::StandardCursor Cursor;
    //    if (_MenuItems[index]->getHoverState() && _MenuItems[index]->getEnabled()) {
    //        if (_MenuItems[index]->getType() == MenuItemType::MButton ||
    //            _MenuItems[index]->getType() == MenuItemType::MSlider ||
    //            _MenuItems[index]->getType() == MenuItemType::MCombobox ||
    //            _MenuItems[index]->getType() == MenuItemType::MLobbyList) {
    //            Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::HAND);
    //            return true;
    //        } else if (_MenuItems[index]->getType() == MenuItemType::MTextbox) {
    //            Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::TEXT);
    //            return true;
    //        } else {
    //            Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::NORMAL);
    //            return false;
    //        }
    //    } else {
    //        Cursor.set(Window.getSystemHandle(), sf::StandardCursor::TYPE::NORMAL);
    //        return false;
    //    }
#endif

//#ifdef SFML_SYSTEM_LINUX
    return false;
//#endif
}
