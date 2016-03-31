#include "stdafx.h"
#include "Menu\MultiplayerMenu.h"

MultiplayerMenu::MultiplayerMenu() : Menu(GameState::MultiplayerSelection), _CreatedLobby(-1)
{			  
	_BackgroundName.setFillColor(sf::Color(0, 0, 0, 100));
	_BackgroundName.setOutlineThickness(1);
	_BackgroundName.setOutlineColor(sf::Color::Black);
	_BackgroundName.setPosition(25, 25);
	_BackgroundName.setSize(sf::Vector2f(550, 65));

	_NameText.setFont(_Font);
	_NameText.setCharacterSize(30);
	_NameText.setPosition(_BackgroundName.getPosition() + sf::Vector2f(20, 12));
	_NameText.setString("Name:");

	std::shared_ptr<Textbox> NameTextBox(new Textbox(_NameText.getPosition() + sf::Vector2f(_NameText.getGlobalBounds().width, 0) + sf::Vector2f(20, 0), sf::Vector2f(200, 40), 30, "Name", true));
	_MenuItems.push_back(NameTextBox);

	_BackgroundJoin.setFillColor(sf::Color(0, 0, 0, 100));
	_BackgroundJoin.setOutlineThickness(1);
	_BackgroundJoin.setOutlineColor(sf::Color::Black);
	_BackgroundJoin.setPosition(_BackgroundName.getPosition() + sf::Vector2f(0, _BackgroundName.getSize().y + 30));
	_BackgroundJoin.setSize(sf::Vector2f(550, 310));

	_JoinText.setFont(_Font);
	_JoinText.setCharacterSize(35);
	_JoinText.setPosition(_BackgroundJoin.getPosition() + sf::Vector2f(20, 10));
	_JoinText.setStyle(sf::Text::Bold);
	_JoinText.setString("Join existing lobby");

	_IPText.setFont(_Font);
	_IPText.setCharacterSize(30);
	_IPText.setPosition(_BackgroundJoin.getPosition() + sf::Vector2f(20, 80));
	_IPText.setString("IP:");

	std::shared_ptr<Textbox> IPTextBox(new Textbox(_BackgroundJoin.getPosition() + sf::Vector2f(200, 80), sf::Vector2f(200, 40), 30, "127.0.0.1", false));
	_MenuItems.push_back(IPTextBox);

	std::shared_ptr<Textbox> PortTextBox(new Textbox(_BackgroundJoin.getPosition() + sf::Vector2f(425, 80), sf::Vector2f(100, 40), 30, "8080", false));
	_MenuItems.push_back(PortTextBox);

	_PasswordText.setFont(_Font);
	_PasswordText.setCharacterSize(30);
	_PasswordText.setPosition(_BackgroundJoin.getPosition() + sf::Vector2f(20, 150));
	_PasswordText.setString("Password:");

	std::shared_ptr<Textbox> PasswordTextBox(new Textbox(_BackgroundJoin.getPosition() + sf::Vector2f(200, 150), sf::Vector2f(200, 40), 30, "Password", false, true));
	_MenuItems.push_back(PasswordTextBox);

	std::shared_ptr<MenuButton> JoinButton(new MenuButton(_BackgroundJoin.getPosition() + sf::Vector2f(120, 220), sf::Vector2f(200, 40), MenuResult::Join, "Join Lobby", TextAlignment::Center));
	_MenuItems.push_back(JoinButton);

	_FeedbackText.setFont(_Font);
	_FeedbackText.setCharacterSize(20);
	_FeedbackText.setPosition(_BackgroundJoin.getPosition() + sf::Vector2f(20, 275));
	_FeedbackText.setColor(sf::Color(220, 0, 0));
	_FeedbackText.setString("Connecting to 127.0.0.1:8080 failed. Please try again.");

	_BackgroundCreate.setFillColor(sf::Color(0, 0, 0, 100));
	_BackgroundCreate.setOutlineThickness(1);
	_BackgroundCreate.setOutlineColor(sf::Color::Black);
	_BackgroundCreate.setPosition(_BackgroundJoin.getPosition() + sf::Vector2f(0, _BackgroundJoin.getSize().y + 30));
	_BackgroundCreate.setSize(sf::Vector2f(550, 200));

	_CreateText.setFont(_Font);
	_CreateText.setCharacterSize(35);
	_CreateText.setPosition(_BackgroundCreate.getPosition() + sf::Vector2f(20, 10));
	_CreateText.setStyle(sf::Text::Bold);
	_CreateText.setString("Create lobby");

	_CreatePassword.setFont(_Font);
	_CreatePassword.setCharacterSize(30);
	_CreatePassword.setPosition(_BackgroundCreate.getPosition() + sf::Vector2f(20, 75));
	_CreatePassword.setString("Password:");

	std::shared_ptr<Textbox> PasswordTextBoxCreate(new Textbox(_BackgroundCreate.getPosition() + sf::Vector2f(200, 75), sf::Vector2f(200, 40), 30, "Password", false, true));
	_MenuItems.push_back(PasswordTextBoxCreate);

	std::shared_ptr<MenuButton> CreateButton(new MenuButton(_BackgroundCreate.getPosition() + sf::Vector2f(120, 145), sf::Vector2f(200, 40), MenuResult::Create, "Create Lobby", TextAlignment::Center));
	_MenuItems.push_back(CreateButton);

	std::shared_ptr<MenuButton> BackButton(new MenuButton(sf::Vector2f(100, 700), sf::Vector2f(150, 50), MenuResult::Back, "Back", TextAlignment::Center));
	_MenuItems.push_back(BackButton);
}

void MultiplayerMenu::render(sf::RenderWindow& Window)
{
	Window.draw(_BackgroundName);
	Window.draw(_BackgroundJoin);
	Window.draw(_BackgroundCreate);

	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}

	Window.draw(_NameText);
	Window.draw(_JoinText);
	Window.draw(_IPText);
	Window.draw(_PasswordText);
	Window.draw(_FeedbackText);
	Window.draw(_CreateText);
	Window.draw(_CreatePassword);
}

GameState MultiplayerMenu::handleEvents(sf::RenderWindow& Window)
{
	checkMenuItemHovered(Window);

	if (Window.pollEvent(_Event))
	{
		return handleMenuItems(_Event);
	}

	return _MenuGameState;
}

GameState MultiplayerMenu::handleMenuItemResult(MenuResult result)
{
	switch (result)
	{
	case MenuResult::Back:
		_CreatedLobby = -1;
		return GameState::Main;
		break;
	case MenuResult::Join:
	{
		_FeedbackText.setColor(sf::Color::White);
		_FeedbackText.setString("Connecting to " + _MenuItems[(int)MenuItemIndex::IP]->getText() + ":" + _MenuItems[(int)MenuItemIndex::Port]->getText() + ". Please wait.");

		_ConnectionThread = std::thread(&NetworkHandle::connect, _NetworkHandle, _MenuItems[(int)MenuItemIndex::IP]->getText(), _MenuItems[(int)MenuItemIndex::PasswordJoin]->getText(), std::stoi(_MenuItems[(int)MenuItemIndex::Port]->getText()), 2.0f);
		_ConnectionThread.detach();

		_CreatedLobby = 0;
		return GameState::Connecting;
	}	
		break;
	case MenuResult::Create:
		_CreatedLobby = 1;
		return GameState::Lobby;
		break;
	default:
		break;
	}

	return _MenuGameState;
}

NetworkCommunication MultiplayerMenu::update(float frametime)
{
	std::lock_guard<std::mutex> lock(_Mutex);
	switch (_NetworkHandle->getLastResponse())
	{
	case NetworkCommunication::ConnectionSuccesfull:
		return NetworkCommunication::ConnectionSuccesfull;
		break;
	case NetworkCommunication::ConnectionFailed:
		_FeedbackText.setColor(sf::Color(220, 0, 0));
		_FeedbackText.setString("Connecting to " + _MenuItems[(int)MenuItemIndex::IP]->getText() + ":" + _MenuItems[(int)MenuItemIndex::Port]->getText() + " failed. Please try again.");

		return NetworkCommunication::ConnectionFailed;
		break;
	case NetworkCommunication::WrongPassword:
		_FeedbackText.setColor(sf::Color(220, 0, 0));
		_FeedbackText.setString("Wrong password. Please try again.");

		return NetworkCommunication::WrongPassword;
		break;
	default:
		break;
	}
}