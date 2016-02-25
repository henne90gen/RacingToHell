#pragma once

class Level 
{
public:
	Level();
	~Level();

	bool update(float FrameTime, bool isRunning);
	void render(sf::RenderWindow& RenderWindow);

	void resetLevel();
	void LevelUp();

	int getDifficulty() { return _Difficulty; }
	int getRoadSpeed() { return (100 * _Difficulty + 100); }

	void resetTimer() { _Timer.restart(); }

private:
	std::vector<sf::Texture*> _Textures;
	sf::Sprite _Sprite;

	sf::Clock _Timer;

	int _Difficulty;
	float _LevelUp;
};