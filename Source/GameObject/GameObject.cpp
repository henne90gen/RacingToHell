#include "stdafx.h"
#include "GameObject\GameObject.h"


GameObject::GameObject(sf::Vector2f pos, GameObjects Type, std::string Filename) : _Type(Type)
{
	//Setting up texture and sprite
	if (_Type == GameObjects::Canister || _Type == GameObjects::Tools || _Type == GameObjects::BulletObjectAI || _Type == GameObjects::BulletObjectPlayer) {
		_Texture.loadFromFile("Resources/Texture/Object/" + Filename + ".png");
	}
	else if (_Type == GameObjects::AI) {
		_Texture.loadFromFile("Resources/Texture/TrafficCar/" + Filename + ".png");
	}
	else if (_Type == GameObjects::Player) {
		_Texture.loadFromFile("Resources/Texture/PlayerCar/" + Filename + ".png");
	}
	_Sprite.setTexture(_Texture);
	_Sprite.setOrigin(_Sprite.getLocalBounds().width / 2, _Sprite.getLocalBounds().height / 2);
	_Sprite.setPosition(pos);

	//Setting up sound
	_SoundBuffer.loadFromFile("Resources/Sound/sample.wav");
	_Sound.setBuffer(_SoundBuffer);
}

GameObject::~GameObject()
{

}

void GameObject::render(sf::RenderWindow& Window)
{
	Window.draw(_Sprite);
	if (_Sound.getStatus() == sf::Sound::Stopped) {
		_Sound.play();
	}
}

void GameObject::setSkin(sf::Texture* skin)
{
	_Texture = *skin;
	_Sprite.setTexture(_Texture);
}
