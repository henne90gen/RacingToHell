#pragma once
class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void render(sf::RenderWindow& Window) = 0;
	virtual void handleEvent(sf::Event Event) = 0;
	virtual void update(float FrameTime) = 0;
};

