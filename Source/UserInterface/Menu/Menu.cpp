#include "stdafx.h"
#include "UserInterface/Menu/Menu.h"
#include "Framework/Framework.h"

Menu::Menu(Framework &framework, GameState menuState) : GameScreen(framework), _MenuGameState(menuState) {
    _Text.setFont(_FW.getOptionsManager().getFont());
}
