#include "stdafx.h"
#include "Menu\Menu.h"


Menu::Menu()
{
	//Menu-Items
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 240), MenuResult::Resume));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2 - 100, SCREENHEIGHT - 335), MenuResult::PreviousSkin));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2 + 37, SCREENHEIGHT - 335), MenuResult::NextSkin));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 320), MenuResult::Exit));

	//Main-Menu Text
	_MainMenuFont.loadFromFile("Resources/Font/arial.ttf");
	_MainMenuText.setFont(_MainMenuFont);
	_MainMenuText.setString("Main Menu");
	_MainMenuText.setCharacterSize(55);
	_MainMenuText.setColor(sf::Color::White);
	_MainMenuText.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _MainMenuText.getLocalBounds().width / 2, 160));

	//Stats-Box
	_StatBox = new StatBox(sf::Vector2f(SCREENWIDTH / 2, 575));
}

Menu::~Menu()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		delete _MenuItems[i];
		_MenuItems[i] = nullptr;
	}

	delete _StatBox;
	_StatBox = nullptr;
}

void Menu::render(sf::RenderWindow & Window, int SelectedCar)
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}
	Window.draw(_CarSkin);
	Window.draw(_MainMenuText);

	_StatBox->render(Window, SelectedCar);
}

void Menu::update(float FrameTime)
{
}

void Menu::checkMouseHover(sf::RenderWindow & Window)
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
