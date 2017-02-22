#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/MenuItem.h"

MenuItem::MenuItem(MenuItemType type, MenuResult action, sf::Font &font) : _Font(font), _Enabled(true), _Type(type),
                                                                           _Action(action), _Focused(false) {
// FIXME why is that doing here?
    sf::Uint32 MyCharset[] = {0x25CF};
}
