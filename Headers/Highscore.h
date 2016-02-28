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

	int MinScore();
	void PlacePlayer(std::string& Name, int Level, int Score);
private:
	sf::RectangleShape _Background;
	sf::Font _Font;
	sf::Text _HeadlineName;
	sf::Text _HeadlineLevel;
	sf::Text _HeadlineScore;

	float _HeadlineNameHeight, _HeadlineLevelWidth, _HeadlineScoreWidth, _Gap;

	std::vector<Player> _PlayerList;
	std::vector<sf::Text> _HighscoreTexts;
	std::string _Filename;

	void SortScoreTable();
	void TestPrintPlayers();

	std::vector<std::string> split(const std::string& s, char delim);
};