#include "stdafx.h"
#include "UserInterface/Menu/MainMenu.h"
#include "Framework/Framework.h"

MainMenu::MainMenu(Framework &framework) : Menu(framework, GameState::MainMenu) {
    sf::Font &font = _FW.getOptionsManager().getFont();

    //Menu-Items
    sf::Vector2f ButtonSize = sf::Vector2f(250, 50);

    std::shared_ptr<MenuButton> playBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 100), ButtonSize,
                                                                       MenuResult::Resume, font, "Play",
                                                                       TextAlignment::Center);
    _MenuItems.push_back(playBtn);

    std::shared_ptr<MenuButton> multiplayerBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 170),
                                                                              ButtonSize, MenuResult::Multiplayer, font,
                                                                              "Multiplayer", TextAlignment::Center);
    multiplayerBtn->setEnabled(false);
    _MenuItems.push_back(multiplayerBtn);

    std::shared_ptr<MenuButton> highscoresBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 240),
                                                                             ButtonSize, MenuResult::Highscore, font,
                                                                             "Highscores", TextAlignment::Center);
    _MenuItems.push_back(highscoresBtn);

    std::shared_ptr<MenuButton> optionsBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 310),
                                                                          ButtonSize, MenuResult::Option, font,
                                                                          "Options",
                                                                          TextAlignment::Center);
    _MenuItems.push_back(optionsBtn);

    std::shared_ptr<MenuButton> exitBtn = std::make_shared<MenuButton>(sf::Vector2f(SCREENWIDTH / 2, 380), ButtonSize,
                                                                       MenuResult::Exit, font, "Exit",
                                                                       TextAlignment::Center);
    _MenuItems.push_back(exitBtn);

    std::shared_ptr<MenuButton> leftBtn = std::make_shared<MenuButton>(
            sf::Vector2f(SCREENWIDTH / 2 - 60, SCREENHEIGHT - 335), sf::Vector2f(0, 50), MenuResult::PreviousSkin, font,
            "<<", TextAlignment::Left);
    _MenuItems.push_back(leftBtn);

    std::shared_ptr<MenuButton> rightBtn = std::make_shared<MenuButton>(
            sf::Vector2f(SCREENWIDTH / 2 + 50, SCREENHEIGHT - 335), sf::Vector2f(0, 50), MenuResult::NextSkin, font,
            ">>", TextAlignment::Left);
    _MenuItems.push_back(rightBtn);

    //MainMenu-Menu Text
    _Text.setString("Main Menu");
    _Text.setCharacterSize(53);
    _Text.setStyle(sf::Text::Style::Bold);
    _Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 20));

    //Stats-Box
    _StatBox = std::make_shared<StatBox>(sf::Vector2f(SCREENWIDTH / 2, 575));
}

void MainMenu::render(sf::RenderWindow &window) {
    _StatBox->setCarStats(_FW.getGOM().getPlayerCar()->getPlayerCarIndex());
    _StatBox->render(window);
    window.draw(_Text);

    Menu::render(window);
}

void MainMenu::handleEvent(sf::Event &event) {
    switch (getMenuItemResult(event)) {
        case MenuResult::Resume:
            _FW.advanceToGameState(GameState::Running);
            break;
        case MenuResult::Highscore:
            _FW.advanceToGameState(GameState::Highscores);
            break;
        case MenuResult::Option:
            _FW.advanceToGameState(GameState::Options);
            break;
        case MenuResult::PreviousSkin:
            _FW.getGOM().previousPlayerCar();
            break;
        case MenuResult::NextSkin:
            _FW.getGOM().nextPlayerCar();
            break;
        case MenuResult::Exit:
            _FW.advanceToGameState(GameState::Exiting);
            break;
        case MenuResult::Multiplayer:
            _FW.advanceToGameState(GameState::MultiplayerSelection);
            break;
        default:
            break;
    }
}
