#pragma once

class Level 
{
public:
	Level();
	~Level();

	void update(float FrameTime);
	void render(sf::RenderWindow& RenderWindow);

	int getDifficulty() { return _Difficulty; }
	int getRoadSpeed() { return (100 * _Difficulty + 100); }
private:
	sf::Texture _Texture;
	sf::Sprite _Sprite;

	int _Difficulty;
	float _TimePassed;
	float _LevelUp;

	void LevelUp();
};