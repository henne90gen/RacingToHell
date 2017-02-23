#include "stdafx.h"
#include "UserInterface/Menu/OptionsMenu.h"
#include "Framework/Framework.h"

OptionsMenu::OptionsMenu(Framework &framework) : Menu(framework, GameState::Options), _ChangeSliderValue() {
    sf::Font &font = _FW.getOptionsManager().getFont();

    _Text.setString("Options");
    _Text.setCharacterSize(53);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 100));

    _VolumeSlider = Slider(sf::Vector2f(SCREENWIDTH / 2 - 100, 200), font, "Volume", MIN_VOLUME, MAX_VOLUME);
    _VolumeSlider.setValue(_FW.getOptionsManager().getVolume());

    _FpsSlider = Slider(sf::Vector2f(SCREENWIDTH / 2 - 100, 250), font, "FPS", MIN_FPS, MAX_FPS);
    _FpsSlider.setValue(_FW.getOptionsManager().getFPS());
    _FpsSlider.setEnabled(!_FW.getOptionsManager().isVSyncEnabled());

    _VSyncCheck = CheckBox(sf::Vector2f(SCREENWIDTH / 2, 300), font, "VSync");
    _VSyncCheck.setValue(_FW.getOptionsManager().isVSyncEnabled());

    std::vector<std::string> difficultyStrings = _FW.getOptionsManager().getDifficultyStrings();
    std::vector<float> difficultyValues = _FW.getOptionsManager().getDifficultyValues();
//    _DifficultyCombo = ComboBox(sf::Vector2f(SCREENWIDTH / 2 - 100, 350), difficultyStrings, difficultyValues, font);
//    if (_FW.getLastGameState(2) == GameState::Running) {
//        _DifficultyCombo.setEnabled(false);
//    }

    std::vector<std::string> gamemodeStrings = _FW.getOptionsManager().getGameModeStrings();
    std::vector<float> gamemodeValues = _FW.getOptionsManager().getGameModeValues();
//    std::shared_ptr<ComboBox> gamemode = std::make_shared<ComboBox>(sf::Vector2f(SCREENWIDTH / 2 - 160, 420),
//                                                                    gamemodeStrings,
//                                                                    gamemodeValues, font, 320);
//    if (_FW.getLastGameState(2) == GameState::Running) {
//        gamemode->setEnabled(false);
//    }

    _ScoreMultiplierBackground.setPosition(sf::Vector2f(SCREENWIDTH / 2 - 150, 480));
    _ScoreMultiplierBackground.setSize(sf::Vector2f(300, 25));
    _ScoreMultiplierBackground.setFillColor(sf::Color(0, 0, 0, 175));
    _ScoreMultiplierBackground.setOutlineThickness(1);
    _ScoreMultiplierBackground.setOutlineColor(sf::Color::Black);

    _ScoreMultiplierText.setFont(font);
    _ScoreMultiplierText.setString("Score: x1.0");
    _ScoreMultiplierText.setCharacterSize(15);
    _ScoreMultiplierText.setPosition(sf::Vector2f(SCREENWIDTH / 2 - 150 + 10, 482.5f));

    _DebugCheck = CheckBox(sf::Vector2f(SCREENWIDTH / 2, 540), font, "Debug");
    _DebugCheck.setValue(_FW.getOptionsManager().isDebugEnabled());

    _CreditsBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 590), sf::Vector2f(150, 50), font, "Credits",
                             TextAlignment::Center);

    _BackBtn = MenuButton(sf::Vector2f(SCREENWIDTH / 2, 650), sf::Vector2f(150, 50), font, "Back",
                          TextAlignment::Center);

    _FPS.setFont(font);
    _FPS.setString("000");
    _FPS.setFillColor(sf::Color::White);
    _FPS.setPosition(sf::Vector2f(_FpsSlider.getRect().left + _FpsSlider.getRect().width + 20,
                                  _FpsSlider.getRect().top - 5));

    _FPSBackground.setPosition(sf::Vector2f(_FPS.getPosition().x - 10, _FPS.getPosition().y - 3));
    _FPSBackground.setSize(sf::Vector2f(_FPS.getLocalBounds().width + 20, _FPS.getLocalBounds().height + 20));
    _FPSBackground.setFillColor(sf::Color(0, 0, 0, 175));
    _FPSBackground.setOutlineThickness(1);
    _FPSBackground.setOutlineColor(sf::Color::Black);

    _Volume.setFont(font);
    _Volume.setString("000");
    _Volume.setFillColor(sf::Color::White);
    _Volume.setPosition(sf::Vector2f(_VolumeSlider.getRect().left + _VolumeSlider.getRect().width + 20,
                                     _VolumeSlider.getRect().top - 5));

    _VolumeBackground.setPosition(sf::Vector2f(_Volume.getPosition().x - 10, _Volume.getPosition().y - 3));
    _VolumeBackground.setSize(sf::Vector2f(_Volume.getLocalBounds().width + 20, _Volume.getLocalBounds().height + 20));
    _VolumeBackground.setFillColor(sf::Color(0, 0, 0, 175));
    _VolumeBackground.setOutlineThickness(1);
    _VolumeBackground.setOutlineColor(sf::Color::Black);
}

void OptionsMenu::render(sf::RenderWindow &renderWindow) {
    renderWindow.draw(_Text);

    renderWindow.draw(_VolumeBackground);
    renderWindow.draw(_Volume);
    _VolumeSlider.render(renderWindow);

    renderWindow.draw(_FPSBackground);
    renderWindow.draw(_FPS);
    _FpsSlider.render(renderWindow);

    _VSyncCheck.render(renderWindow);

    renderWindow.draw(_ScoreMultiplierBackground);
    renderWindow.draw(_ScoreMultiplierText);

    _DebugCheck.render(renderWindow);

    _CreditsBtn.render(renderWindow);
    _BackBtn.render(renderWindow);
}

void OptionsMenu::handleEvent(sf::Event &event) {
    if (menuItemTriggered(event, _VolumeSlider)) {
        _FW.getOptionsManager().setVolume(_VolumeSlider.getValue());
    } else if (menuItemTriggered(event, _FpsSlider)) {
        _FW.getOptionsManager().setFPS(_FpsSlider.getValue());
    } else if (menuItemTriggered(event, _VSyncCheck)) {
        _FW.getOptionsManager().setVSyncEnabled(_VSyncCheck.getValue());
        _FpsSlider.setEnabled(!_VSyncCheck.getValue());
//    } else if (menuItemTriggered(event, _DifficultyCombo)) {
//            _FW.getOptionsManager().setDifficulty(_DifficultyCombo.getValue());
//    } else if (menuItemTriggered(event, _GameModeCombo)) {
//            _FW.getOptionsManager().setGameMode(_GameModeCombo.getValue());
    } else if (menuItemTriggered(event, _DebugCheck)) {
        _FW.getOptionsManager().setDebugEnabled(_DebugCheck.getValue());
        _FW.reloadGameScreens();
    } else if (menuItemTriggered(event, _CreditsBtn)) {
        _FW.advanceToGameState(GameState::About);
    } else if (menuItemTriggered(event, _BackBtn)) {
        _FW.getOptionsManager().save();
        _FW.goBackGameState();
    }
}

void OptionsMenu::update(float FrameTime) {
    // Update and center text
    _FpsSlider.setValue(_FW.getOptionsManager().getFPS());
    _FPS.setString(rh::floatToString(_FW.getOptionsManager().getFPS(), 0));
    _FPS.setPosition(sf::Vector2f(_FPSBackground.getPosition().x + _FPSBackground.getLocalBounds().width / 2.0f -
                                  _FPS.getLocalBounds().width / 2.0f,
                                  _FPSBackground.getPosition().y + _FPSBackground.getLocalBounds().height / 2.0f -
                                  _FPS.getLocalBounds().height + 2));

    // Update and center text
    _VolumeSlider.setValue(_FW.getOptionsManager().getVolume());
    _Volume.setString(
            std::to_string((int) (_VolumeSlider.getValue() * 100 / _VolumeSlider.getMaxValue())));
    _Volume.setPosition(sf::Vector2f(
            _VolumeBackground.getPosition().x + _VolumeBackground.getLocalBounds().width / 2.0f -
            _Volume.getLocalBounds().width / 2.0f,
            _VolumeBackground.getPosition().y + _VolumeBackground.getLocalBounds().height / 2.0f -
            _Volume.getLocalBounds().height + 2));

    // Get current multiplier
    float multiplier = _FW.getOptionsManager().getScoreMultiplier();
    _ScoreMultiplierText.setString(
            "Score: x" + rh::floatToString(multiplier, 2));

    // Update difficulty and GameMode
//    _MenuItems[DifficultyIndex]->setValue((float) _FW.getOptionsManager().getDifficulty());
//    _MenuItems[GameModeIndex]->setValue((float) _FW.getOptionsManager().getGameMode());

    // Change color of multiplier text
    if (multiplier == 1.0f) {
        _ScoreMultiplierText.setFillColor(sf::Color::White);
    } else if (multiplier < 1.0f) {
        _ScoreMultiplierText.setFillColor(sf::Color::Red);
    } else {
        _ScoreMultiplierText.setFillColor(sf::Color::Green);
    }
}
