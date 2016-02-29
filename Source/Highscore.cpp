#include "stdafx.h"
#include "Highscore.h"

Highscore::Highscore(sf::Vector2f Position) : _Gap(42)
{
	_Background.setFillColor(sf::Color(0, 0, 0, 100));
	_Background.setSize(sf::Vector2f(450, 500));
	_Background.setPosition(Position);
	
	Position += sf::Vector2f(15, 15);

	_Font.loadFromFile("Resources/Font/arial.ttf");

	_HeadlineRank.setFont(_Font);
	_HeadlineRank.setCharacterSize(30);
	_HeadlineRank.setStyle(sf::Text::Style::Bold);
	_HeadlineRank.setPosition(Position);
	_HeadlineRank.setString("Rank");
	_HeadlineRankWidth = _HeadlineRank.getLocalBounds().width;

	_HeadlineName = _HeadlineRank;
	_HeadlineName.setPosition(_HeadlineRank.getPosition() + sf::Vector2f(_HeadlineRank.getLocalBounds().width + 10, 0));
	_HeadlineName.setString("Name");
	_HeadlineNameHeight = _HeadlineName.getLocalBounds().height + 30;

	_HeadlineLevel = _HeadlineRank;
	_HeadlineLevel.setString("Level");
	_HeadlineLevel.setPosition(_HeadlineName.getPosition() + sf::Vector2f(_HeadlineName.getLocalBounds().width + 70, 0));
	_HeadlineLevelWidth = _HeadlineLevel.getLocalBounds().width;

	_HeadlineScore = _HeadlineRank;
	_HeadlineScore.setPosition(_HeadlineLevel.getPosition() + sf::Vector2f(_HeadlineLevel.getLocalBounds().width + 30, 0));
	_HeadlineScore.setString("Score");
	_HeadlineScoreWidth = _HeadlineScore.getLocalBounds().width;

	_Filename = "Resources/Data/Highscore.txt";

	loadScoreTable();
}

Highscore::~Highscore()
{

}

void Highscore::render(sf::RenderWindow& RenderWindow)
{
	RenderWindow.draw(_Background);
	RenderWindow.draw(_HeadlineRank);
	RenderWindow.draw(_HeadlineName);
	RenderWindow.draw(_HeadlineLevel);
	RenderWindow.draw(_HeadlineScore);
	
	int numPlayers = _PlayerList.size();

	for (unsigned int i = 0; i < numPlayers; i++)
	{
		_HighscoreTexts[i].setString(std::to_string(_PlayerList[i].Rank));
		_HighscoreTexts[i].setOrigin(sf::Vector2f(_HighscoreTexts[i].getLocalBounds().width / 2.0f, 0));
		_HighscoreTexts[i].setPosition(_HeadlineRank.getPosition() + sf::Vector2f(_HeadlineRankWidth / 2.0f, _HeadlineNameHeight + i * _Gap));

		_HighscoreTexts[i + numPlayers].setString(_PlayerList[i].Name);
		_HighscoreTexts[i + numPlayers].setPosition(_HeadlineName.getPosition() + sf::Vector2f(0, _HeadlineNameHeight + i * _Gap));

		_HighscoreTexts[i + 2 * numPlayers].setString(std::to_string(_PlayerList[i].Level));
		_HighscoreTexts[i + 2 * numPlayers].setPosition(_HeadlineLevel.getPosition() + sf::Vector2f(_HeadlineLevelWidth - _HighscoreTexts[i + 2 * numPlayers].getLocalBounds().width, _HeadlineNameHeight + i * _Gap));

		_HighscoreTexts[i + 3 * numPlayers].setString(std::to_string(_PlayerList[i].Score));
		_HighscoreTexts[i + 3 * numPlayers].setPosition(_HeadlineScore.getPosition() + sf::Vector2f(_HeadlineScoreWidth - _HighscoreTexts[i + 3 * numPlayers].getLocalBounds().width, _HeadlineNameHeight + i * _Gap));

		if (_NewHighscore) {
			if (_PlayerList[i].Score == _Score) {
				_HighscoreTexts[i].setColor(sf::Color::Yellow);
				_HighscoreTexts[i + numPlayers].setColor(sf::Color::Yellow);
				_HighscoreTexts[i + 2 * numPlayers].setColor(sf::Color::Yellow);
			}
		}
		else {

		}

		RenderWindow.draw(_HighscoreTexts[i]);
		RenderWindow.draw(_HighscoreTexts[i + numPlayers]);
		RenderWindow.draw(_HighscoreTexts[i + 2 * numPlayers]);
		RenderWindow.draw(_HighscoreTexts[i + 3 * numPlayers]);
	}
}

void Highscore::loadScoreTable()
{
	std::vector<std::string> PlayerInformations;
	std::string PlayerInformation;

	std::ifstream FileStream;
	FileStream.open(_Filename);

	while (std::getline(FileStream, PlayerInformation))
	{
		PlayerInformations.push_back(PlayerInformation);
	}

	_PlayerList.clear();

	for (unsigned int i = 0; i < PlayerInformations.size(); i++)
	{
		std::vector<std::string> Info = split(PlayerInformations[i], ';');
		
		if (Info.size() == 3)
		{
			Player newPlayer;
			newPlayer.Rank = i + 1;
			newPlayer.Name = Info[0];
			newPlayer.Level = std::stoi(Info[1]);
			newPlayer.Score = std::stoi(Info[2]);

			_PlayerList.push_back(newPlayer);
		}
	}

	FileStream.close();

	SortScoreTable();

	for (unsigned int i = 0; i < _PlayerList.size() * 4; i++)
	{
		sf::Text HighscoreText;
		HighscoreText.setFont(_Font);
		HighscoreText.setCharacterSize(25);

		_HighscoreTexts.push_back(HighscoreText);
	}
}

void Highscore::SaveScoreTable()
{
	std::ofstream FileStream;
	FileStream.open(_Filename);

	for (unsigned int i = 0; i < _PlayerList.size(); i++)
	{
		FileStream << _PlayerList[i].Name << ";" << _PlayerList[i].Level << ";" << _PlayerList[i].Score << std::endl;
	}

	FileStream.close();
}

void Highscore::SortScoreTable()
{
	std::sort(_PlayerList.rbegin(), _PlayerList.rend());
}

std::vector<std::string> Highscore::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string number;
	while (std::getline(ss, number, delim)) {
		elems.push_back(number);
	}
	return elems;
}

void Highscore::PlacePlayer(std::string& Name, int Level)
{
	if (_PlayerList.size() == 10)
	{
		_PlayerList.erase(_PlayerList.begin() + _PlayerList.size() - 1);
	}
	
	Player newPlayer;
	newPlayer.Name = Name;
	newPlayer.Level = Level;
	newPlayer.Score = _Score;

	_PlayerList.push_back(newPlayer);

	SortScoreTable();
}

void Highscore::setScore(int Score) { 
	_Score = Score;
	if (_Score > MinScore()) {
		_NewHighscore = true;
	}
	else {
		_NewHighscore = false;
	}
}

int Highscore::MinScore()
{
	if (_PlayerList.size() < 10)
	{
		return 0;
	}
	else
	{
		return std::min_element(_PlayerList.begin(), _PlayerList.end())->Score;
	}
}