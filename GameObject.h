#pragma once

#include <string>

class GameObject
{
public:
	GameObject(std::string Filename);
	~GameObject();

	virtual void render(sf::RenderWindow& Window);
	virtual void handleEvent(sf::Event& Event) = 0;
	virtual void update(float FrameTime) = 0;

	sf::Vector2f getPos() { return  _Sprite.getPosition(); }
	void setPos(sf::Vector2f Pos) { _Sprite.setPosition(Pos); }
	float getWidth() { return _Sprite.getLocalBounds().width; }
	float getHeight() { return _Sprite.getLocalBounds().height; }
private:
	sf::Sprite _Sprite;
	sf::Texture _Texture;
};

