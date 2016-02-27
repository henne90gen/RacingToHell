#include "stdafx.h"
#include "Menu/OptionsMenu.h"

OptionsMenu::OptionsMenu(sf::SoundBuffer& MusicBuffer) : Menu(MusicBuffer)
{
	_MenuItems.push_back(new MenuItem(sf::Vector2f(SCREENWIDTH / 2, 320), MenuResult::Back));

	_Text.setString("Main Menu");
	_Text.setCharacterSize(53);
	_Text.setColor(sf::Color::White);
	_Text.setStyle(sf::Text::Style::Bold);
	_Text.setPosition(sf::Vector2f(SCREENWIDTH / 2 - _Text.getLocalBounds().width / 2, 160));

	if (_Font.loadFromFile("Resources/Font/arial.ttf")) {
		_VolumeText.setFont(_Font);
		_VolumeText.setString("Volume:");
		_VolumeText.setPosition(sf::Vector2f(100,200));
	}
	_VolumeLine.setFillColor(sf::Color::Black);
	_VolumeLine.setSize(sf::Vector2f(100, 5));
	_VolumeLine.setPosition(sf::Vector2f(_VolumeText.getPosition().x + _VolumeText.getLocalBounds().width + 10, 
		_VolumeText.getPosition().y + _VolumeText.getLocalBounds().height - 1));

	_VolumeSlider.setFillColor(sf::Color::White);
	_VolumeSlider.setSize(sf::Vector2f(10,30));
	_VolumeSlider.setPosition(sf::Vector2f(_VolumeLine.getPosition().x + _VolumeLine.getLocalBounds().width, 
		_VolumeLine.getPosition().y + _VolumeLine.getLocalBounds().height / 2.0f - _VolumeSlider.getLocalBounds().height / 2.0f));
}


OptionsMenu::~OptionsMenu()
{
	for (int i = 0; i < _MenuItems.size(); i++) {
		delete _MenuItems[i];
		_MenuItems[i] = nullptr;
	}
}

void OptionsMenu::update(float FrameTime)
{
}

void OptionsMenu::render(sf::RenderWindow & Window)
{
	Window.draw(_VolumeText);
	Window.draw(_VolumeLine);
	Window.draw(_VolumeSlider);

	for (int i = 0; i < _MenuItems.size(); i++) {
		_MenuItems[i]->render(Window);
	}

	Window.draw(_Text);
}

GameState OptionsMenu::handleEvents(sf::RenderWindow & Window)
{
	while (Window.pollEvent(_Event)) {
		checkMouseHover(Window);
		if (_Event.type == sf::Event::MouseButtonPressed) {
			sf::Vector2f MousePos = sf::Vector2f(_Event.mouseButton.x, _Event.mouseButton.y);
			for (int i = 0; i < getMenuItems().size(); i++) {
				sf::FloatRect rect = getMenuItems()[i]->getRect();
				if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) {
					switch (getMenuItems()[i]->getAction()) {
					case MenuResult::Back:
						return GameState::Main;
						break;
					case MenuResult::Nothing:
						return GameState::Pause;
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
	return GameState::Options;
}

void OptionsMenu::updateVolume()
{

}
