#include "stdafx.h"
#include "Menu\MainMenu.h"

MainMenu::MainMenu(sf::SoundBuffer& MusicBuffer) : Menu(MusicBuffer)
{
	//Menu-Items
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2 - 100, SCREENHEIGHT - 335), MenuResult::PreviousSkin));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2 + 37, SCREENHEIGHT - 335), MenuResult::NextSkin));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 240), MenuResult::Resume));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 310), MenuResult::Option));
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 380), MenuResult::Exit));

	//Main-Menu Text
	_Text.setString("Main Menu");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));

	//Stats-Box
	_StatBox = new StatBox(sf::Vector2f(SCREENWIDTH / 2, 575));
}

MainMenu::~MainMenu()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		delete _MenuItems[i];
		_MenuItems[i] = nullptr;
	}

	delete _StatBox;
	_StatBox = nullptr;
}

void MainMenu::render(sf::RenderWindow & Window, int SelectedCar)
{
	Window.draw(_CarSkin);
	_StatBox->render(Window, SelectedCar);

	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}

	Window.draw(_Text);
}

GameState MainMenu::handleEvents(sf::RenderWindow & Window)
{
	return GameState::Main;
}

GameState MainMenu::handleEvents(sf::RenderWindow& Window, int& carSkinIndex)
{
	while (Window.pollEvent(_Event)) {
		checkMouseHover(Window);
		if (_Event.type == sf::Event::MouseButtonPressed) {
			sf::Vector2f MousePos = sf::Vector2f(_Event.mouseButton.x, _Event.mouseButton.y);
			for (int i = 0; i < getMenuItems().size(); i++) {
				sf::FloatRect rect = getMenuItems()[i]->getRect();
				if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) {
					switch (getMenuItems()[i]->getAction()) {
					case MenuResult::Resume:
						std::cout << "Resuming" << std::endl;
						return GameState::Running;
						break;
					case MenuResult::Option:
						return GameState::Options;
						break;
					case MenuResult::Nothing:
						return GameState::Pause;
						break;
					case MenuResult::PreviousSkin:
						carSkinIndex--;
						break;
					case MenuResult::NextSkin:
						carSkinIndex++;
						break;
					case MenuResult::Exit:
						return GameState::Exiting;
						break;
					}
				}
			}
		}
		else if (_Event.type == sf::Event::Closed) {
			return GameState::Exiting;
		}
	}
	return GameState::Main;
}

void MainMenu::playMusic()
{
	if (_Music.getStatus() == sf::Sound::Stopped) {
		_Music.play();
	}
}

void MainMenu::stopMusic()
{
	_Music.stop();
}

void MainMenu::pauseMusic()
{
	_Music.pause();
}

void MainMenu::update(float FrameTime)
{
}

void MainMenu::render(sf::RenderWindow & Window)
{
}
