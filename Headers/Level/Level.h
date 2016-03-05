#pragma once

class Level 
{
public:
	Level();
	~Level();

	bool update(float FrameTime, GameState gameState);
	void render(sf::RenderWindow& RenderWindow);
	void playMusic();
	void stopMusic();
	void pauseMusic();
	void setVolume(float Volume);

	void resetLevel();
	void levelUp();

	void load();

	int getDifficulty() { return _Difficulty; }
	int getRoadSpeed() { return (100 * _Difficulty + 100); }

	void resetTimer() { _Timer.restart(); }

private:
	std::vector<sf::Texture*> _Textures;
	sf::Sprite _Sprite;

	sf::SoundBuffer _MusicBuffer;
	sf::Sound _Music;

	sf::Clock _Timer;

	int _Difficulty;
	float _LevelUp;
};