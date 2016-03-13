#include "stdafx.h"
#include "Menu/OptionsMenu.h"

OptionsMenu::OptionsMenu() : Menu(GameState::Options)
{
	_MenuItems.push_back(new Slider(sf::Vector2f(sf::Vector2f(SCREENWIDTH / 2 - 100, 250)), MenuResult::Nothing, "FPS", 0.0f, 210.0f));
	_MenuItems.push_back(new Slider(sf::Vector2f(sf::Vector2f(SCREENWIDTH / 2 - 100, 300)), MenuResult::Nothing, "Volume", 0.0f, 5.0f));
	_MenuItems.push_back(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 350), sf::Vector2f(150, 50), MenuResult::Back, "Back", TextAlignment::Center));

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

	if (_MenuItems[_JoystickSelection]->getType() == MenuItems::MSlider) {
		Slider* slider = dynamic_cast<Slider*>(_MenuItems[_JoystickSelection]);
		slider->setValue(slider->getValue() + slider->getMaxValue() * _ChangeSliderValue * FrameTime);
		_ChangeSliderValue = 0;
	}
}

void OptionsMenu::saveOptions()
{
	std::string Path = "Resources/Data/Settings.txt";
	std::ofstream FileStream;
	FileStream.open(Path);

	FileStream << getFPS() << std::endl << getVolume();

	FileStream.close();
}
