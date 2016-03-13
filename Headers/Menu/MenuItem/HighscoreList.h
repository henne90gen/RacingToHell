#pragma once

#include <fstream>
#include <sstream>
#include <algorithm>
#include "Serializable.h"

#include "Serializable.h"

class HighscoreList
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

		virtual void operator>>(std::ofstream & stream)
		{
			write(stream, Rank);
			write(stream, Name);
			write(stream, Level);
			write(stream, Score);
		}

		virtual void operator<<(std::ifstream & stream)
		{
			read(stream, Rank);
			read(stream, Name);
			read(stream, Level);
			read(stream, Score);
		}
	};

	HighscoreList(sf::Vector2f Position);
	~HighscoreList();

	void render(sf::RenderWindow& RenderWindow);

	void loadScoreTable();
	void SaveScoreTable();

	int getScore() { return _CurrentScore; }
	void setScore(int Score) { _CurrentScore = Score; }

	void PlacePlayer(std::string& Name, int Level);
private:
	sf::RectangleShape _Background;
	sf::Font _Font;
	sf::Text _HeadlineRank;
	sf::Text _HeadlineName;
	sf::Text _HeadlineLevel;
	sf::Text _HeadlineScore;

	float _HeadlineRankWidth, _HeadlineNameHeight, _HeadlineLevelWidth, _HeadlineScoreWidth, _Gap;
	std::string _CurrentName;
	int _CurrentScore, _CurrentLevel, _NumScores;

	std::vector<Player> _PlayerList;
	std::vector<sf::Text> _HighscoreTexts;
	std::string _Filename;

	void SortScoreTable();
	void TestPrintPlayers();
};