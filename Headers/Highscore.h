#pragma once

#include <fstream>
#include <sstream>
#include <algorithm>
#include "Serializable.h"

#include "Serializable.h"

class Highscore
{
public:
	struct Player : public Serializable
	{
		int Rank;
		std::string Name;
		int Score;
		int Level;

		bool operator<(Player& const Player2) const
		{
			return Score < Player2.Score;
		}

		std::ofstream& operator>>(std::ofstream & stream)
		{
			write(stream, Rank);
			write(stream, Name);
			write(stream, Level);
			write(stream, Score);

			return stream;
		}

		std::ifstream& operator<<(std::ifstream & stream)
		{
			read(stream, Rank);
			read(stream, Name);
			read(stream, Level);
			read(stream, Score);

			return stream;
		}
	};

	Highscore(sf::Vector2f Position);
	~Highscore();

	void render(sf::RenderWindow& RenderWindow);

	void loadScoreTable();
	void SaveScoreTable();

	int MinScore();
	void PlacePlayer(std::string& Name, int Level);

	void setScore(int Score);
	int getScore() { return _Score; }

	bool isNewHighscore() { return _NewHighscore; }
private:
	sf::RectangleShape _Background;
	sf::Font _Font;
	sf::Text _HeadlineRank;
	sf::Text _HeadlineName;
	sf::Text _HeadlineLevel;
	sf::Text _HeadlineScore;

	float _HeadlineRankWidth, _HeadlineNameHeight, _HeadlineLevelWidth, _HeadlineScoreWidth, _Gap;
	int _Score;
	bool _NewHighscore;

	std::vector<Player> _PlayerList;
	std::vector<sf::Text> _HighscoreTexts;
	std::string _Filename;

	void SortScoreTable();
	void TestPrintPlayers();

	std::vector<std::string> split(const std::string& s, char delim);
};