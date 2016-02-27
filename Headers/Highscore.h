#pragma once

#include <fstream>


class Highscore
{
public:
	struct Player
	{
		std::string _Name;
		int _Score;
		int _Level;
	};

	Highscore(sf::Vector2f Position);
	~Highscore();

	void render(sf::RenderWindow& RenderWindow);
private:
	std::ofstream HighscoreFile;

	sf::RectangleShape _Background;
	sf::Font _Font;
	sf::Text _TextNames;
	sf::Text _TextLevel;
	sf::Text _TextScore;

	std::vector<Player> _PlayerList;
};