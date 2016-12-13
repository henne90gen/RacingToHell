#include "stdafx.h"
#include "Screen/Menu/OptionsMenu.h"
#include "Framework/Framework.h"

OptionsMenu::OptionsMenu(Framework &framework, GameState returnState) : Menu(framework, GameState::Options,
                                                                             returnState), _ChangeSliderValue() {
    _ScoreMultiplierList = {1.0f, 0.0f, 0.5f, 3.f};

    // FIXME create constants for maximum and minimum fps
    std::shared_ptr<Slider> fps(
            new Slider(sf::Vector2f(sf::Vector2f(SCREENWIDTH / 2 - 100, 250)), MenuResult::SliderChange, "FPS", MIN_FPS,
                       MAX_FPS));
    fps->setValue(_FW.getOptionsManager().getFPS());
    _MenuItems.push_back(fps);

    std::shared_ptr<Slider> volume(
            new Slider(sf::Vector2f(sf::Vector2f(SCREENWIDTH / 2 - 100, 300)), MenuResult::SliderChange, "Volume", 0.0f,
                       5.0f));
    _MenuItems.push_back(volume);

    std::vector<std::string> difficulties = {"Easy", "Normal", "Hard", "Insane"};
    std::shared_ptr<ComboBox> difficulty(
            new ComboBox(sf::Vector2f(SCREENWIDTH / 2 - 100, 350), difficulties, MenuResult::Nothing));
    _MenuItems.push_back(difficulty);

    std::vector<std::string> gamemodes = {"Standard", "Invincibility", "Infinite Energy", "Hardcore"};
    std::shared_ptr<ComboBox> gamemode(
            new ComboBox(sf::Vector2f(SCREENWIDTH / 2 - 160, 420), gamemodes, MenuResult::Nothing, 320));
    _MenuItems.push_back(gamemode);

    _ScoreMultiplierBackground.setPosition(sf::Vector2f(SCREENWIDTH / 2 - 150, 480));
    _ScoreMultiplierBackground.setSize(sf::Vector2f(300, 25));
    _ScoreMultiplierBackground.setFillColor(sf::Color(0, 0, 0, 175));
    _ScoreMultiplierBackground.setOutlineThickness(1);
    _ScoreMultiplierBackground.setOutlineColor(sf::Color::Black);

    sf::Font &font = _FW.getOptionsManager().getFont();

    _ScoreMultiplierText.setFont(font);
    _ScoreMultiplierText.setString("Score: x1.0");
    _ScoreMultiplierText.setCharacterSize(15);
    _ScoreMultiplierText.setColor(sf::Color::White);
    _ScoreMultiplierText.setPosition(sf::Vector2f(SCREENWIDTH / 2 - 150 + 10, 480 + 2.5));

    std::shared_ptr<MenuButton> button1(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 520), sf::Vector2f(150, 50), MenuResult::Credits, "Credits",
                           TextAlignment::Center));
    _MenuItems.push_back(button1);

    std::shared_ptr<MenuButton> button2(
            new MenuButton(sf::Vector2f(SCREENWIDTH / 2, 590), sf::Vector2f(150, 50), MenuResult::Back, "Back",
                           TextAlignment::Center));
    _MenuItems.push_back(button2);

    _Text.setString("Options");
    _Text.setCharacterSize(53);
    _Text.setColor(sf::Color::White);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));

    _FPS.setFont(font);
    _FPS.setString("000");
    _FPS.setPosition(sf::Vector2f(_MenuItems[FPS]->getRect().left + _MenuItems[FPS]->getRect().width + 20,
                                  _MenuItems[FPS]->getRect().top - 5));

    _FPSBackground.setPosition(sf::Vector2f(_FPS.getPosition().x - 10, _FPS.getPosition().y - 3));
    _FPSBackground.setSize(sf::Vector2f(_FPS.getLocalBounds().width + 20, _FPS.getLocalBounds().height + 20));
    _FPSBackground.setFillColor(sf::Color(0, 0, 0, 175));
    _FPSBackground.setOutlineThickness(1);
    _FPSBackground.setOutlineColor(sf::Color::Black);

    _Volume.setFont(font);
    _Volume.setString("000");
    _Volume.setPosition(sf::Vector2f(_MenuItems[Volume]->getRect().left + _MenuItems[Volume]->getRect().width + 20,
                                     _MenuItems[Volume]->getRect().top - 5));

    _VolumeBackground.setPosition(sf::Vector2f(_Volume.getPosition().x - 10, _Volume.getPosition().y - 3));
    _VolumeBackground.setSize(sf::Vector2f(_Volume.getLocalBounds().width + 20, _Volume.getLocalBounds().height + 20));
    _VolumeBackground.setFillColor(sf::Color(0, 0, 0, 175));
    _VolumeBackground.setOutlineThickness(1);
    _VolumeBackground.setOutlineColor(sf::Color::Black);
}

void OptionsMenu::render(sf::RenderWindow &window) {
    window.draw(_Text);
    window.draw(_FPSBackground);
    window.draw(_FPS);
    window.draw(_VolumeBackground);
    window.draw(_Volume);
    window.draw(_ScoreMultiplierBackground);
    window.draw(_ScoreMultiplierText);

    Menu::render(window);
}

void OptionsMenu::handleEvent(sf::Event &event) {

//		float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
//
//		if (_Event.type == sf::Event::JoystickButtonPressed) {
//			if (sf::Joystick::isButtonPressed(0, 1)) {
//				return _ReturnState;
//			}
//		}
//
//		handleJoystick(Y);

    _FW.setGameState(handleMenuItems(event));
}

GameState OptionsMenu::handleMenuItemResult(MenuResult result) {
    switch (result) {
        case MenuResult::SliderChange:
            _FW.getOptionsManager().setFPS(_MenuItems[FPS]->getValue());
            _FW.getOptionsManager().setVolume(_MenuItems[Volume]->getValue());
            break;
        case MenuResult::Back:
            return _ReturnGameState;
        case MenuResult::Credits:
            return GameState::About;
    }
    return _MenuGameState;
}

void OptionsMenu::update(float FrameTime) {
//	float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
//	if (X < -80) {
//		_ChangeSliderValue = -1;
//	}
//	else if (X > 80) {
//		_ChangeSliderValue = 1;
//	}

//	_MenuItems[_JoystickSelection]->setValue(_MenuItems[_JoystickSelection]->getValue() + _MenuItems[_JoystickSelection]->getMaxValue() * _ChangeSliderValue * FrameTime);
//    _ChangeSliderValue = 0;

    // Update and center text
    _MenuItems[FPS]->setValue(_FW.getOptionsManager().getFPS());
    _FPS.setString(floatToString(_FW.getOptionsManager().getFPS(), 0));
    _FPS.setPosition(sf::Vector2f(_FPSBackground.getPosition().x + _FPSBackground.getLocalBounds().width / 2.0f -
                                  _FPS.getLocalBounds().width / 2.0f,
                                  _FPSBackground.getPosition().y + _FPSBackground.getLocalBounds().height / 2.0f -
                                  _FPS.getLocalBounds().height + 2));

    // Update and center text
    _Volume.setString(std::to_string((int) (_MenuItems[Volume]->getValue() * 100 / _MenuItems[Volume]->getMaxValue())));
    _Volume.setPosition(sf::Vector2f(
            _VolumeBackground.getPosition().x + _VolumeBackground.getLocalBounds().width / 2.0f -
            _Volume.getLocalBounds().width / 2.0f,
            _VolumeBackground.getPosition().y + _VolumeBackground.getLocalBounds().height / 2.0f -
            _Volume.getLocalBounds().height + 2));


    _ScoreMultiplierText.setString(
            "Score: x" + floatToString(_ScoreMultiplierList[(int) _FW.getOptionsManager().getGameMode()], 1));

    if (_ScoreMultiplierList[(int) _FW.getOptionsManager().getGameMode()] == 1.0f) {
        _ScoreMultiplierText.setColor(sf::Color::White);
    } else if (_ScoreMultiplierList[(int) _FW.getOptionsManager().getGameMode()] < 1.0f) {
        _ScoreMultiplierText.setColor(sf::Color::Red);
    } else {
        _ScoreMultiplierText.setColor(sf::Color::Green);
    }
}


std::string OptionsMenu::floatToString(float value, unsigned int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}
