#pragma once

class Level 
{
public:
	Level();
	~Level();

	void update(float FrameTime);
	void render(sf::RenderWindow& RenderWindow);

private:
	sf::Texture _Texture;
	sf::Sprite _Sprite;

	int _Difficulty;
};