#include "stdafx.h"
#include "UserInterface/Menu/MultiplayerMenu.h"
#include "Framework/Framework.h"

MultiplayerMenu::MultiplayerMenu(Framework &framework) : Menu(framework, GameState::MultiplayerSelection),
                                                         _CreatedLobby(-1) {
//    _BackgroundName.setFillColor(sf::Color(0, 0, 0, 100));
//    _BackgroundName.setOutlineThickness(1);
//    _BackgroundName.setOutlineColor(sf::Color::Black);
//    _BackgroundName.setPosition(25, 25);
//    _BackgroundName.setSize(sf::Vector2f(550, 65));
//
//    sf::Font &font = _FW.getOptionsManager().getFont();
//
//    _NameText.setFont(font);
//    _NameText.setCharacterSize(30);
//    _NameText.setPosition(_BackgroundName.getPosition() + sf::Vector2f(20, 12));
//    _NameText.setString("Name:");
//
//    std::shared_ptr<Textbox> NameTextBox = std::make_shared<Textbox>(_NameText.getPosition() + sf::Vector2f(_NameText.getGlobalBounds().width, 0) + sf::Vector2f(20, 0),
//            sf::Vector2f(200, 40), 30, "", MenuResult::Nothing, font, true));
//    _MenuItems.push_back(NameTextBox);
//
//    _BackgroundJoin.setFillColor(sf::Color(0, 0, 0, 100));
//    _BackgroundJoin.setOutlineThickness(1);
//    _BackgroundJoin.setOutlineColor(sf::Color::Black);
//    _BackgroundJoin.setPosition(_BackgroundName.getPosition() + sf::Vector2f(0, _BackgroundName.getSize().y + 30));
//    _BackgroundJoin.setSize(sf::Vector2f(550, 310));
//
//    _JoinText.setFont(font);
//    _JoinText.setCharacterSize(35);
//    _JoinText.setPosition(_BackgroundJoin.getPosition() + sf::Vector2f(20, 10));
//    _JoinText.setStyle(sf::Text::Bold);
//    _JoinText.setString("Join existing lobby");
//
//    _IPText.setFont(font);
//    _IPText.setCharacterSize(30);
//    _IPText.setPosition(_BackgroundJoin.getPosition() + sf::Vector2f(20, 80));
//    _IPText.setString("IP:");
//
//    std::shared_ptr<Textbox> IPTextBox(
//            new Textbox(_BackgroundJoin.getPosition() + sf::Vector2f(170, 80), sf::Vector2f(240, 40), 30, "127.0.0.1",
//                        MenuResult::Nothing, false));
//    _MenuItems.push_back(IPTextBox);
//
//    std::shared_ptr<Textbox> PortTextBox(
//            new Textbox(_BackgroundJoin.getPosition() + sf::Vector2f(440, 80), sf::Vector2f(100, 40), 30, "12345",
//                        MenuResult::Nothing, false));
//    _MenuItems.push_back(PortTextBox);
//
//    _PasswordText.setFont(font);
//    _PasswordText.setCharacterSize(30);
//    _PasswordText.setPosition(_BackgroundJoin.getPosition() + sf::Vector2f(20, 150));
//    _PasswordText.setString("Password:");
//
//    std::shared_ptr<Textbox> PasswordTextBox(
//            new Textbox(_BackgroundJoin.getPosition() + sf::Vector2f(170, 150), sf::Vector2f(240, 40), 30, "",
//                        MenuResult::Nothing, false, true));
//    _MenuItems.push_back(PasswordTextBox);
//
//    std::shared_ptr<MenuButton> JoinButton(
//            new MenuButton(_BackgroundJoin.getPosition() + sf::Vector2f(120, 220), sf::Vector2f(200, 40),
//                           MenuResult::Join, "Join Lobby", TextAlignment::Center));
//    _MenuItems.push_back(JoinButton);
//
//    _FeedbackText.setFont(font);
//    _FeedbackText.setCharacterSize(20);
//    _FeedbackText.setPosition(_BackgroundJoin.getPosition() + sf::Vector2f(20, 275));
//
//    _BackgroundCreate.setFillColor(sf::Color(0, 0, 0, 100));
//    _BackgroundCreate.setOutlineThickness(1);
//    _BackgroundCreate.setOutlineColor(sf::Color::Black);
//    _BackgroundCreate.setPosition(_BackgroundJoin.getPosition() + sf::Vector2f(0, _BackgroundJoin.getSize().y + 30));
//    _BackgroundCreate.setSize(sf::Vector2f(550, 225));
//
//    _CreateText.setFont(font);
//    _CreateText.setCharacterSize(35);
//    _CreateText.setPosition(_BackgroundCreate.getPosition() + sf::Vector2f(20, 10));
//    _CreateText.setStyle(sf::Text::Bold);
//    _CreateText.setString("Create lobby");
//
//    _CreatePassword.setFont(font);
//    _CreatePassword.setCharacterSize(30);
//    _CreatePassword.setPosition(_BackgroundCreate.getPosition() + sf::Vector2f(20, 75));
//    _CreatePassword.setString("Password:");
//
//    std::shared_ptr<Textbox> PasswordTextBoxCreate(
//            new Textbox(_BackgroundCreate.getPosition() + sf::Vector2f(170, 75), sf::Vector2f(240, 40), 30, "",
//                        MenuResult::Nothing, false, true));
//    _MenuItems.push_back(PasswordTextBoxCreate);
//
//    std::shared_ptr<Textbox> PortCreate(
//            new Textbox(_BackgroundCreate.getPosition() + sf::Vector2f(440, 75), sf::Vector2f(100, 40), 30, "12345",
//                        MenuResult::Nothing, false, false));
//    _MenuItems.push_back(PortCreate);
//
//    std::shared_ptr<MenuButton> CreateButton(
//            new MenuButton(_BackgroundCreate.getPosition() + sf::Vector2f(120, 145), sf::Vector2f(200, 40),
//                           MenuResult::Create, "Create Lobby", TextAlignment::Center));
//    _MenuItems.push_back(CreateButton);
//
//    _FeedbackTextCreate.setFont(font);
//    _FeedbackTextCreate.setCharacterSize(20);
//    _FeedbackTextCreate.setPosition(_BackgroundCreate.getPosition() + sf::Vector2f(20, 195));
//
//    std::shared_ptr<MenuButton> BackButton(
//            new MenuButton(sf::Vector2f(100, 725), sf::Vector2f(150, 50), MenuResult::Back, "Back",
//                           TextAlignment::Center));
//    _MenuItems.push_back(BackButton);
}

void MultiplayerMenu::render(sf::RenderWindow &window) {
    window.draw(_BackgroundName);
    window.draw(_BackgroundJoin);
    window.draw(_BackgroundCreate);

    window.draw(_NameText);
    window.draw(_JoinText);
    window.draw(_IPText);
    window.draw(_PasswordText);
    window.draw(_FeedbackText);
    window.draw(_CreateText);
    window.draw(_CreatePassword);
    window.draw(_FeedbackTextCreate);

    Menu::render(window);
}

void MultiplayerMenu::handleEvent(sf::Event &event) {
    switch (getMenuItemResult(event)) {
        case MenuResult::Back:
            _CreatedLobby = -1;
            _FW.advanceToGameState(GameState::MainMenu);
            break;
        case MenuResult::Join: {
            if (_MenuItems[(int) MenuItemIndex::Name]->getText() == "") {
                _FeedbackText.setFillColor(sf::Color(220, 0, 0));
                _FeedbackText.setString("Please enter a name.");
            } else if (_MenuItems[(int) MenuItemIndex::IP]->getText() == "") {
                _FeedbackText.setFillColor(sf::Color(220, 0, 0));
                _FeedbackText.setString("Please enter an ip address.");
            } else if (_MenuItems[(int) MenuItemIndex::Port]->getText() == "") {
                _FeedbackText.setFillColor(sf::Color(220, 0, 0));
                _FeedbackText.setString("Please enter a port.");
            } else {
                _FeedbackText.setFillColor(sf::Color::White);
                _FeedbackText.setString("Connecting to " + _MenuItems[(int) MenuItemIndex::IP]->getText() + ":" +
                                        _MenuItems[(int) MenuItemIndex::Port]->getText() + ". Please wait.");

                _NetworkHandle->setState(NetworkState::NoNetState);

                /*_ConnectionThread = std::thread(&NetworkHandle::connect, _NetworkHandle,
                                                _MenuItems[(int) MenuItemIndex::IP]->getText(),
                                                _MenuItems[(int) MenuItemIndex::PasswordJoin]->getText(),
                                                _MenuItems[(int) MenuItemIndex::Name]->getText(),
                                                std::stoi(_MenuItems[(int) MenuItemIndex::Port]->getText()), 2.0f);
                _ConnectionThread.detach();*/

                _CreatedLobby = 0;
                _MenuGameState = GameState::Connecting;
            }
            _FW.advanceToGameState(_MenuGameState);
        }
            break;
        case MenuResult::Create:
            _CreatedLobby = 1;

            if (_MenuItems[(int) MenuItemIndex::Name]->getText() == "") {
                _FeedbackTextCreate.setFillColor(sf::Color(220, 0, 0));
                _FeedbackTextCreate.setString("Please enter a name.");
            } else if (_MenuItems[(int) MenuItemIndex::PortCreate]->getText() == "") {
                _FeedbackTextCreate.setFillColor(sf::Color(220, 0, 0));
                _FeedbackTextCreate.setString("Please enter a port.");
            } else {
                if (_NetworkHandle->create(_MenuItems[(int) MenuItemIndex::Name]->getText(),
                                           _MenuItems[(int) MenuItemIndex::PasswordCreate]->getText(),
                                           std::stoi(_MenuItems[(int) MenuItemIndex::PortCreate]->getText()))) {
                    std::cout << "Lobby opened with password '"
                              << _MenuItems[(int) MenuItemIndex::PasswordCreate]->getText() << "', listening on port: "
                              << _MenuItems[(int) MenuItemIndex::PortCreate]->getText() << std::endl;
                    _FW.advanceToGameState(GameState::Lobby);
                } else {
                    _FeedbackTextCreate.setFillColor(sf::Color(220, 0, 0));
                    _FeedbackTextCreate.setString("Unable to bind listener socket to port " +
                                                  _MenuItems[(int) MenuItemIndex::PortCreate]->getText() + ".");
                }
            }

            _FW.advanceToGameState(GameState::MultiplayerSelection);
            break;
        default:
            break;
    }
}

void MultiplayerMenu::update(float frametime) {
//	std::lock_guard<std::mutex> lock(_Mutex);
//	switch (_NetworkHandle->getLastResponse().first)
//	{
//	case NetworkCommunication::ConnectionSuccesfull:
//		_MenuGameState = GameState::MultiplayerSelection;
//		_FeedbackText.setString("");
//		return NetworkCommunication::ConnectionSuccesfull;
//		break;
//	case NetworkCommunication::ConnectionFailed:
//		_FeedbackText.setColor(sf::Color(220, 0, 0));
//		_FeedbackText.setString("Connecting to " + _MenuItems[(int)MenuItemIndex::IP]->getText() + ":" + _MenuItems[(int)MenuItemIndex::Port]->getText() + " failed.");
//
//		_MenuGameState = GameState::MultiplayerSelection;
//		return NetworkCommunication::ConnectionFailed;
//		break;
//	case NetworkCommunication::WrongPassword:
//		_FeedbackText.setColor(sf::Color(220, 0, 0));
//		_FeedbackText.setString("Wrong password. Please try again.");
//
//		_MenuGameState = GameState::MultiplayerSelection;
//		return NetworkCommunication::WrongPassword;
//		break;
//	default:
//		return NetworkCommunication::NoNetComm;
//		break;
//	}
}

void MultiplayerMenu::setPlayerName(std::string name) {
    _MenuItems[0]->setText(name);
}

std::string MultiplayerMenu::getPlayerName() {
    return _MenuItems[0]->getText();
}

void MultiplayerMenu::resetFeedback() {
    _FeedbackText.setString("");
    _FeedbackTextCreate.setString("");
}

void MultiplayerMenu::resetTextbox() {
    _MenuItems[(int) MenuItemIndex::IP]->setText("");
    _MenuItems[(int) MenuItemIndex::Port]->setText("");
    _MenuItems[(int) MenuItemIndex::PasswordJoin]->setText("");
}

void MultiplayerMenu::setKickMessage() {
    _FeedbackText.setFillColor(sf::Color(220, 0, 0));
    _FeedbackText.setString("You have been kicked by the host.");
}

void MultiplayerMenu::setLobbyClosedMessage() {
    _FeedbackText.setFillColor(sf::Color(220, 0, 0));
    _FeedbackText.setString("The lobby was closed by the host.");
}
