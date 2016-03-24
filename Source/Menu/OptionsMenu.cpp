#include "stdafx.h"
#include "Menu/OptionsMenu.h"

OptionsMenu::OptionsMenu() : Menu(GameState::Options)
{
	std::shared_ptr<Slider> slider1(new Slider(sf::Vector2f(sf::Vector2f(SCREENWIDTH / 2 - 100, 250)), MenuResult::Nothing, "FPS", 0.0f, 210.0f));
	_MenuItems.push_back(slider1);
	std::shared_ptr<Slider> slider2(new Slider(sf::Vector2f(sf::Vector2f(SCREENWIDTH / 2 - 100, 300)), MenuResult::Nothing, "Volume", 0.0f, 5.0f));
	_MenuItems.push_back(slider2);
	std::shared_ptr<MenuButton> button1(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 350), sf::Vector2f(150, 50), MenuResult::Credits, "Credits", TextAlignment::Center));
	_MenuItems.push_back(button1);
	std::shared_ptr<MenuButton> button2(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 420), sf::Vector2f(150, 50), MenuResult::Back, "Back", TextAlignment::Center));
	_MenuItems.push_back(button2);

	_Text.setString("Options");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));
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

		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

		if (_Event.type == sf::Event::JoystickButtonPressed) {
			if (sf::Joystick::isButtonPressed(0, 1)) {
				return _ReturnState;
			}
		}

		handleJoystick(Y);

		return handleMenuItems(_Event);
	}
	return _MenuGameState;
}

GameState OptionsMenu::handleMenuItemResult(MenuResult result)
{
	switch (result) {
	case MenuResult::Back:
		saveOptions();
		return _ReturnState;
		break;
	case MenuResult::Credits:
		return GameState::About;
		break;
	}
	return _MenuGameState;
}

void OptionsMenu::update(float FrameTime)
{
	float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
	if (X < -80) {
		_ChangeSliderValue = -1;
	}
	else if (X > 80) {
		_ChangeSliderValue = 1;
	}

	if (_MenuItems[_JoystickSelection]->setValue(_MenuItems[_JoystickSelection]->getValue() + _MenuItems[_JoystickSelection]->getMaxValue() * _ChangeSliderValue * FrameTime)) {
		_ChangeSliderValue = 0;
	}
}

void OptionsMenu::saveOptions()
{
	std::string Path = "Resources/Data/Settings.cfg";
	std::ofstream FileStream;
	FileStream.open(Path);

	FileStream << getFPS() << std::endl << getVolume();

	FileStream.close();
}
