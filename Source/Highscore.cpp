#include "stdafx.h"
#include "Highscore.h"

Highscore::Highscore(sf::Vector2f Position)
{
	_Background.setFillColor(sf::Color(0, 0, 0, 100));
	_Background.setSize(sf::Vector2f(450, 500));
	_Background.setPosition(Position);

	_Font.loadFromFile("Resources\\Font\\arial.ttf");

	_HeadlineName.setFont(_Font);
	_HeadlineName.setCharacterSize(30);
	_HeadlineName.setStyle(sf::Text::Style::Bold);
	_HeadlineName.setPosition(Position + sf::Vector2f(20,20));
	_HeadlineName.setString("Name");
	_HeadlineNameHeight = _HeadlineName.getLocalBounds().height;

	_HeadlineLevel = _HeadlineName;
	_HeadlineLevel.setPosition(Position + sf::Vector2f(_Background.getSize().x / 2 - _HeadlineLevel.getLocalBounds().width / 2, 20));
	_HeadlineLevel.setString("Level");
	_HeadlineLevelWidth = _HeadlineLevel.getLocalBounds().width;

	_HeadlineScore = _HeadlineName;
	_HeadlineScore.setPosition(Position + sf::Vector2f(_Background.getSize().x - _HeadlineScore.getLocalBounds().width - 20, 20));
	_HeadlineScore.setString("Score");
	_HeadlineScoreWidth = _HeadlineScore.getLocalBounds().width;

	_Filename = "Resources\Data\Highscore.txt";

	loadScoreTable();
}

Highscore::~Highscore()
{

}

void Highscore::render(sf::RenderWindow& RenderWindow)
{
	RenderWindow.draw(_Background);
	RenderWindow.draw(_HeadlineName);
	RenderWindow.draw(_HeadlineLevel);
	RenderWindow.draw(_HeadlineScore);
	
	for (unsigned int i = 0; i < _PlayerList.size(); i++)
	{
		_HighscoreTexts[i].setString(_PlayerList[i].Name);
		_HighscoreTexts[i].setPosition(_HeadlineName.getPosition() + sf::Vector2f(0, _HeadlineNameHeight + 30 + i * (_HighscoreTexts[i].getLocalBounds().height + 20)));

		_HighscoreTexts[i + _PlayerList.size()].setString(std::to_string(_PlayerList[i].Level));
		_HighscoreTexts[i + _PlayerList.size()].setPosition(_HeadlineLevel.getPosition() + sf::Vector2f(_HeadlineLevelWidth - _HighscoreTexts[i + _PlayerList.size()].getLocalBounds().width, _HeadlineNameHeight + 30 + i * (_HighscoreTexts[i].getLocalBounds().height + 20)));

		_HighscoreTexts[i + 2 * _PlayerList.size()].setString(std::to_string(_PlayerList[i].Score));
		_HighscoreTexts[i + 2 * _PlayerList.size()].setPosition(_HeadlineScore.getPosition() + sf::Vector2f(_HeadlineScoreWidth - _HighscoreTexts[i + 2 * _PlayerList.size()].getLocalBounds().width, _HeadlineNameHeight + 30 + i * (_HighscoreTexts[i].getLocalBounds().height + 20)));

		RenderWindow.draw(_HighscoreTexts[i]);
		RenderWindow.draw(_HighscoreTexts[i + _PlayerList.size()]);
		RenderWindow.draw(_HighscoreTexts[i + 2 * _PlayerList.size()]);
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
			newPlayer.Name = Info[0];
			newPlayer.Level = std::stoi(Info[1]);
			newPlayer.Score = std::stoi(Info[2]);

			_PlayerList.push_back(newPlayer);
		}
	}

	FileStream.close();

	SortScoreTable();

	for (unsigned int i = 0; i < _PlayerList.size() * 3; i++)
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

