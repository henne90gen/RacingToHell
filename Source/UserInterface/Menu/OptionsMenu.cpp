#include "stdafx.h"
#include "UserInterface/Menu/OptionsMenu.h"
#include "Framework/Framework.h"

OptionsMenu::OptionsMenu(Framework &framework) : Menu(framework, GameState::Options), _ChangeSliderValue() {

    sf::Font &font = _FW.getOptionsManager().getFont();

    std::shared_ptr<Slider> fps = std::make_shared<Slider>(sf::Vector2f(SCREENWIDTH / 2 - 100, 250),
                                                           MenuResult::FPSChange, font, "FPS", MIN_FPS, MAX_FPS);
    fps->setValue(_FW.getOptionsManager().getFPS());
    _MenuItems.push_back(fps);

    std::shared_ptr<CheckBox> vSync = std::make_shared<CheckBox>(sf::Vector2f(SCREENWIDTH / 2, 200),
                                                                 MenuResult::VSyncChanged, font, "VSync");
    vSync->setValue(_FW.getOptionsManager().isVSyncEnabled());
    _MenuItems.push_back(vSync);

    std::shared_ptr<Slider> volume = std::make_shared<Slider>(sf::Vector2f(SCREENWIDTH / 2 - 100, 300),
                                                              MenuResult::VolumeChange, font, "Volume", MIN_VOLUME,
                                                              MAX_VOLUME);
    volume->setValue(_FW.getOptionsManager().getVolume());
    _MenuItems.push_back(volume);

    std::vector<std::string> difficultyStrings = _FW.getOptionsManager().getDifficultyStrings();
    std::vector<float> difficultyValues = _FW.getOptionsManager().getDifficultyValues();
    std::shared_ptr<ComboBox> difficulty = std::make_shared<ComboBox>(sf::Vector2f(SCREENWIDTH / 2 - 100, 350),
                                                                      difficultyStrings,
                                                                      difficultyValues, MenuResult::DifficultyChange,
                                                                      font);
    _MenuItems.push_back(difficulty);

    std::vector<std::string> gamemodeStrings = _FW.getOptionsManager().getGameModeStrings();
    std::vector<float> gamemodeValues = _FW.getOptionsManager().getGameModeValues();
    std::shared_ptr<ComboBox> gamemode = std::make_shared<ComboBox>(sf::Vector2f(SCREENWIDTH / 2 - 160, 420),
                                                                    gamemodeStrings,
                                                                    gamemodeValues, MenuResult::GameModeChange, font,
                                                                    320);
    _MenuItems.push_back(gamemode);

    _ScoreMultiplierBackground.setPosition(sf::Vector2f(SCREENWIDTH / 2 - 150, 480));
    _ScoreMultiplierBackground.setSize(sf::Vector2f(300, 25));
    _ScoreMultiplierBackground.setFillColor(sf::Color(0, 0, 0, 175));
    _ScoreMultiplierBackground.setOutlineThickness(1);
    _ScoreMultiplierBackground.setOutlineColor(sf::Color::Black);

    _ScoreMultiplierText.setFont(font);
    _ScoreMultiplierText.setString("Score: x1.0");
    _ScoreMultiplierText.setCharacterSize(15);
    _ScoreMultiplierText.setPosition(sf::Vector2f(SCREENWIDTH / 2 - 150 + 10, 482.5f));

    std::shared_ptr<CheckBox> debug = std::make_shared<CheckBox>(sf::Vector2f(SCREENWIDTH / 2, 540),
                                                                 MenuResult::DebugChange, font,
                                                                 "Debug");
    debug->setValue(_FW.getOptionsManager().isDebugEnabled());
    _MenuItems.push_back(debug);

    std::shared_ptr<MenuButton> creditsBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 590),
                                                                          sf::Vector2f(150, 50),
                                                                          MenuResult::Credits, font, "Credits",
                                                                          TextAlignment::Center);
    _MenuItems.push_back(creditsBtn);

    std::shared_ptr<MenuButton> backBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 650),
                                                                       sf::Vector2f(150, 50),
                                                                       MenuResult::Back, font, "Back",
                                                                       TextAlignment::Center);
    _MenuItems.push_back(backBtn);

    _Text.setString("Options");
    _Text.setCharacterSize(53);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));

    _FPS.setFont(font);
    _FPS.setString("000");
    _FPS.setFillColor(sf::Color::White);
    _FPS.setPosition(sf::Vector2f(_MenuItems[FPSIndex]->getRect().left + _MenuItems[FPSIndex]->getRect().width + 20,
                                  _MenuItems[FPSIndex]->getRect().top - 5));

    _FPSBackground.setPosition(sf::Vector2f(_FPS.getPosition().x - 10, _FPS.getPosition().y - 3));
    _FPSBackground.setSize(sf::Vector2f(_FPS.getLocalBounds().width + 20, _FPS.getLocalBounds().height + 20));
    _FPSBackground.setFillColor(sf::Color(0, 0, 0, 175));
    _FPSBackground.setOutlineThickness(1);
    _FPSBackground.setOutlineColor(sf::Color::Black);

    _Volume.setFont(font);
    _Volume.setString("000");
    _Volume.setFillColor(sf::Color::White);
    _Volume.setPosition(
            sf::Vector2f(_MenuItems[VolumeIndex]->getRect().left + _MenuItems[VolumeIndex]->getRect().width + 20,
                         _MenuItems[VolumeIndex]->getRect().top - 5));

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
    switch (getMenuItemResult(event)) {
        case MenuResult::FPSChange:
            _FW.getOptionsManager().setFPS(_MenuItems[FPSIndex]->getValue());
            break;
        case MenuResult::VSyncChanged:
            _FW.getOptionsManager().setVSyncEnabled((bool) _MenuItems[VSyncIndex]->getValue());
        case MenuResult::VolumeChange:
            _FW.getOptionsManager().setVolume(_MenuItems[VolumeIndex]->getValue());
            break;
        case MenuResult::DifficultyChange:
            _FW.getOptionsManager().setDifficulty((Difficulty) (int) _MenuItems[DifficultyIndex]->getValue());
            break;
        case MenuResult::GameModeChange:
            _FW.getOptionsManager().setGameMode((GameMode) (int) _MenuItems[GameModeIndex]->getValue());
            break;
        case MenuResult::DebugChange:
            _FW.getOptionsManager().setDebugEnabled((bool) _MenuItems[DebugIndex]->getValue());
            _FW.reloadGameScreens();
            break;
        case MenuResult::Credits:
            _FW.advanceToGameState(GameState::About);
            break;
        case MenuResult::Back:
            _FW.getOptionsManager().save();
            _FW.goBackGameState();
            break;
        default:
            break;
    }
}

void OptionsMenu::update(float FrameTime) {
    // Update and center text
    _MenuItems[FPSIndex]->setValue(_FW.getOptionsManager().getFPS());
    _FPS.setString(rh::floatToString(_FW.getOptionsManager().getFPS(), 0));
    _FPS.setPosition(sf::Vector2f(_FPSBackground.getPosition().x + _FPSBackground.getLocalBounds().width / 2.0f -
                                  _FPS.getLocalBounds().width / 2.0f,
                                  _FPSBackground.getPosition().y + _FPSBackground.getLocalBounds().height / 2.0f -
                                  _FPS.getLocalBounds().height + 2));

    // Update and center text
    _MenuItems[VolumeIndex]->setValue(_FW.getOptionsManager().getVolume());
    _Volume.setString(
            std::to_string((int) (_MenuItems[VolumeIndex]->getValue() * 100 / _MenuItems[VolumeIndex]->getMaxValue())));
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
    _MenuItems[DifficultyIndex]->setValue((float) _FW.getOptionsManager().getDifficulty());
    _MenuItems[GameModeIndex]->setValue((float) _FW.getOptionsManager().getGameMode());

    // Change color of multiplier text
    if (multiplier == 1.0f) {
        _ScoreMultiplierText.setFillColor(sf::Color::White);
    } else if (multiplier < 1.0f) {
        _ScoreMultiplierText.setFillColor(sf::Color::Red);
    } else {
        _ScoreMultiplierText.setFillColor(sf::Color::Green);
    }
}
