#pragma once

class Level 
{
public:
	Level();
	~Level();

	void update(float FrameTime);
	void render(sf::RenderWindow& RenderWindow);
	void playMusic();
	void stopMusic();

private:
	sf::Texture _Texture;
	sf::Sprite _Sprite;

	sf::SoundBuffer _MusicBuffer;
	sf::Sound _Music;

	int _Difficulty;
};