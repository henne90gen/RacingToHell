#pragma once

/*
	General interface for every object that's going to be in the game
*/
class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void render(sf::RenderWindow& Window);
	virtual void handleEvent(sf::Event& Event) = 0;
	virtual void update(float FrameTime) = 0;

protected:
	sf::Sprite _Sprite;
	sf::Texture _Texture;
	bool _CustomRenderer;
};

