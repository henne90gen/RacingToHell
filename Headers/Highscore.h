#pragma once

#include <fstream>
#include <sstream>
#include <algorithm>

class Highscore
{
public:
	struct Player
	{
		std::string Name;
		int Score;
		int Level;

		bool operator<(Player& const Player2) const 
		{
			return Score < Player2.Score;
		}
	};

	Highscore(sf::Vector2f Position);
	~Highscore();

	void render(sf::RenderWindow& RenderWindow);

	void loadScoreTable();
	void SaveScoreTable();
private:
	sf::RectangleShape _Background;
	sf::Font _Font;
	sf::Text _TextNames;
	sf::Text _TextLevel;
	sf::Text _TextScore;

	std::vector<Player> _PlayerList;
	std::string _Filename;

	void SortScoreTable();

	std::vector<std::string> split(const std::string& s, char delim)
};