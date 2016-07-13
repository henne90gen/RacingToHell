#include "stdafx.h"
#include "Menu/OptionsMenu.h"

OptionsMenu::OptionsMenu() : Menu(GameState::Options), _ReturnState(), _ChangeSliderValue()
{
	std::shared_ptr<Slider> slider1(new Slider(sf::Vector2f(sf::Vector2f(SCREENWIDTH / 2 - 100, 250)), MenuResult::Nothing, "FPS", 0.0f, 210.0f));
	_MenuItems.push_back(slider1);

	std::shared_ptr<Slider> slider2(new Slider(sf::Vector2f(sf::Vector2f(SCREENWIDTH / 2 - 100, 300)), MenuResult::Nothing, "Volume", 0.0f, 5.0f));
	_MenuItems.push_back(slider2);

	std::vector<std::string> difficulties = { "Easy", "Normal", "Hard", "Insane" };
	std::shared_ptr<ComboBox> combobox(new ComboBox(sf::Vector2f(SCREENWIDTH / 2 - 100, 350), difficulties, MenuResult::Nothing));
	_MenuItems.push_back(combobox);

	std::vector<std::string> gamemodes = { "Standard", "InfiniteEnergy", "Onehit" };
	std::shared_ptr<ComboBox> combobox2(new ComboBox(sf::Vector2f(SCREENWIDTH / 2 - 150, 420), gamemodes, MenuResult::Nothing, 300));
	_MenuItems.push_back(combobox2);

	_ScoreMultiplierBackground.setPosition(sf::Vector2f(SCREENWIDTH / 2 - 150, 480));
	_ScoreMultiplierBackground.setSize(sf::Vector2f(300, 25));
	_ScoreMultiplierBackground.setFillColor(sf::Color(0, 0, 0, 175));
	_ScoreMultiplierBackground.setOutlineThickness(1);
	_ScoreMultiplierBackground.setOutlineColor(sf::Color::Black);

	_ScoreMultiplierText.setFont(_Font);
	_ScoreMultiplierText.setString("Score: x1.0");
	_ScoreMultiplierText.setCharacterSize(15);
	_ScoreMultiplierText.setColor(sf::Color::White);
	_ScoreMultiplierText.setPosition(sf::Vector2f(SCREENWIDTH / 2 - 150 + 10, 480 + 2.5));

	std::shared_ptr<MenuButton> button1(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 520), sf::Vector2f(150, 50), MenuResult::Credits, "Credits", TextAlignment::Center));
	_MenuItems.push_back(button1);

	std::shared_ptr<MenuButton> button2(new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 590), sf::Vector2f(150, 50), MenuResult::Back, "Back", TextAlignment::Center));
	_MenuItems.push_back(button2);

	_Text.setString("Options");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));

	_FPS.setFont(_Font);
	_FPS.setString("000");
	_FPS.setPosition(sf::Vector2f(_MenuItems[FPS]->getRect().left + _MenuItems[FPS]->getRect().width + 20, _MenuItems[FPS]->getRect().top - 5));
	
	_FPSBackground.setPosition(sf::Vector2f(_FPS.getPosition().x - 10, _FPS.getPosition().y - 3));
	_FPSBackground.setSize(sf::Vector2f(_FPS.getLocalBounds().width + 20, _FPS.getLocalBounds().height + 20));
	_FPSBackground.setFillColor(sf::Color(0, 0, 0, 175));
	_FPSBackground.setOutlineThickness(1);
	_FPSBackground.setOutlineColor(sf::Color::Black);

	_Volume.setFont(_Font);
	_Volume.setString("000");
	_Volume.setPosition(sf::Vector2f(_MenuItems[Volume]->getRect().left + _MenuItems[Volume]->getRect().width + 20, _MenuItems[Volume]->getRect().top - 5));
	
	_VolumeBackground.setPosition(sf::Vector2f(_Volume.getPosition().x - 10, _Volume.getPosition().y - 3));
	_VolumeBackground.setSize(sf::Vector2f(_Volume.getLocalBounds().width + 20, _Volume.getLocalBounds().height + 20));
	_VolumeBackground.setFillColor(sf::Color(0, 0, 0, 175));
	_VolumeBackground.setOutlineThickness(1);
	_VolumeBackground.setOutlineColor(sf::Color::Black);
}

void OptionsMenu::render(sf::RenderWindow & window)
{
	window.draw(_Text);
	window.draw(_FPSBackground);
	window.draw(_FPS);
	window.draw(_VolumeBackground);
	window.draw(_Volume);
	window.draw(_ScoreMultiplierBackground);
	window.draw(_ScoreMultiplierText);

	Menu::render(window);
}

GameState OptionsMenu::handleEvents(sf::RenderWindow & Window)
{
	if (Window.pollEvent(_Event)) {

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

	_MenuItems[_JoystickSelection]->setValue(_MenuItems[_JoystickSelection]->getValue() + _MenuItems[_JoystickSelection]->getMaxValue() * _ChangeSliderValue * FrameTime);
	_ChangeSliderValue = 0;

	_FPS.setString(std::to_string((int)getFPS()));
	_FPS.setPosition(sf::Vector2f(	_FPSBackground.getPosition().x + _FPSBackground.getLocalBounds().width / 2.0f - _FPS.getLocalBounds().width / 2.0f,
									_FPSBackground.getPosition().y + _FPSBackground.getLocalBounds().height / 2.0f - _FPS.getLocalBounds().height + 2));
	
	_Volume.setString(std::to_string((int)(_MenuItems[Volume]->getValue() * 100 / _MenuItems[Volume]->getMaxValue())));
	_Volume.setPosition(sf::Vector2f(	_VolumeBackground.getPosition().x + _VolumeBackground.getLocalBounds().width / 2.0f - _Volume.getLocalBounds().width / 2.0f,
										_VolumeBackground.getPosition().y + _VolumeBackground.getLocalBounds().height / 2.0f - _Volume.getLocalBounds().height + 2));

	std::vector<std::string> MultiplierValues = { "1.0", "0.4", "2.0" };
	_ScoreMultiplierText.setString("Score: x" + MultiplierValues[getMultiplier()]);
}

void OptionsMenu::loadOptions(MultiplayerMenu& mpMenu) {
	std::vector<std::string> Settings;
	std::string Option;
	std::ifstream FileStream;

	FileStream.open("Resources/Data/Settings.cfg");
	while (std::getline(FileStream, Option))
	{
		Settings.push_back(Option);
	}
	FileStream.close();

	if (Settings.size() >= 3)
	{
		setFPS(std::stoi(Settings[0]));
		setVolume(std::stof(Settings[1]));
		setDifficulty(std::stoi(Settings[2]));

		if (Settings.size() >= 4)
		{
			mpMenu.setPlayerName(Settings[3]);
		}
	} 
}

void OptionsMenu::saveOptions(MultiplayerMenu& mpMenu)
{
	std::string Path = "Resources/Data/Settings.cfg";
	std::ofstream FileStream;
	FileStream.open(Path);

	FileStream << getFPS() << std::endl << getVolume() << std::endl << getDifficulty() << std::endl << mpMenu.getPlayerName();

	FileStream.close();
}
