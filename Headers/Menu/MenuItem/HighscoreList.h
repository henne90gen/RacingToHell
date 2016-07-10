#pragma once

#include <fstream>
#include <sstream>
#include <algorithm>

#include "Serializable.h"

class HighscoreList
{
public:
	/*
		Player representation containing the rank, name, level and score of a player.
		All those variables can be written and also read back to and from a file.
	*/
	struct Player : public Serializable
	{
		int Rank;
		std::string Name;
		int Score;
		int Level;

		bool operator<(Player& Player2) const
		{
			return Score < Player2.Score;
		}

		void operator>>(std::ostream & stream)
		{
			write(stream, Rank);
			write(stream, Name);
			write(stream, Level);
			write(stream, Score);
		}

		void operator<<(std::istream & stream)
		{
			read(stream, Rank);
			read(stream, Name);
			read(stream, Level);
			read(stream, Score);
		}

		void operator>>(sf::Packet& packet)
		{
			write(packet, Rank);
			write(packet, Name);
			write(packet, Level);
			write(packet, Score);
		}

		void operator<<(sf::Packet& packet)
		{
			read(packet, Rank);
			read(packet, Name);
			read(packet, Level);
			read(packet, Score);
		}
	};

	/*
		Prints out the ranks 1-10 of the highscore list.
		If the new highscore didn't make the top 10 it will be displayed below with the corresponding rank
		@param pos Position of the HighscoreList on screen
	*/
	HighscoreList(sf::Vector2f pos);
	~HighscoreList() {}

	/*
		Renders the HighscoreList to the specified RenderWindow
		@param window Window to draw to
	*/
	void render(sf::RenderWindow& window);

	/*
		Loads the highscore list from file
	*/
	void loadScoreTable();
	
	/*
		Saves the HighscoreList to file
	*/
	void saveScoreTable();

	/*
			@return int Score of the last round played
	*/
	int getScore() { return _CurrentScore; }
	
	/*
		Sets the score of the last round
		@param score Score of the last round
	*/
	void setScore(int score) { _CurrentScore = score; }

	/*
		Adds the player of the last round to the HighscoreList
		@param name Name of the player
		@param level Level the player reached
	*/
	void placePlayer(std::string& name, int level);
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

	/*
		Sorts the player in the score table by score
	*/
	void sortScoreTable();
};
