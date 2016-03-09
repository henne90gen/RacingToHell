#pragma once

#include <fstream>
#include <sstream>
#include <algorithm>

#include "Serializable.h"

class Highscore
{
public:
	struct Player : public Serializable
	{
		int Rank;
		std::string Name;
		int Level;
		int Score;

		bool operator<(Player& const Player2) const 
		{
			return Score < Player2.Score;
		}

		virtual void serialize(std::ostream & stream)
		{
			stream.write((char*)&Rank, sizeof(Rank));
			stream.write((char*)&Level, sizeof(Level));
			stream.write((char*)&Score, sizeof(Score));
			int length = Name.size();
			stream.write((char*)&length, sizeof(length));
			stream << Name;
		}

		virtual void deserialize(std::istream & stream)
		{
			stream.read((char*)&Rank, sizeof(Rank));
			stream.read((char*)&Level, sizeof(Level));
			stream.read((char*)&Score, sizeof(Score));
			int length;
			stream.read((char*)&length, sizeof(length));
			char* newString = new char[length];
			stream.read(newString, length);
			for (int i = 0; i < length; i++) {
				Name += newString[i];
			}
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