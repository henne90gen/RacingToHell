#pragma once

class Level 
{
public:
	Level();
	~Level();

	void update(float FrameTime, bool isRunning);
	void render(sf::RenderWindow& RenderWindow);

	void resetDifficulty() { _Difficulty = 1; }

	int getDifficulty() { return _Difficulty; }
	int getRoadSpeed() { return (100 * _Difficulty + 100); }
private:
	std::vector<sf::Texture*> _Textures;
	sf::Sprite _Sprite;

	int _Difficulty;
	float _TimePassed;
	float _LevelUp;

	void LevelUp();
};