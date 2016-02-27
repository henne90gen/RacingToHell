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

	_Filename = "Resources\\Data\\Highscore.txt";

	loadScoreTable();
	SortScoreTable();
	TestPrintPlayers();
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
		sf::Text TextName;
		sf::Text TextLevel;
		sf::Text TextScore;

		TextName.setFont(_Font);
		TextName.setCharacterSize(25);
		TextName.setString(_PlayerList[i].Name);
		TextName.setPosition(_HeadlineName.getPosition() + sf::Vector2f(0, _HeadlineNameHeight + 30 + i * (TextName.getLocalBounds().height + 20)));
	
		TextLevel.setFont(_Font);
		TextLevel.setCharacterSize(25);
		TextLevel.setString(std::to_string(_PlayerList[i].Level));
		TextLevel.setPosition(_HeadlineLevel.getPosition() + sf::Vector2f(_HeadlineLevelWidth - TextLevel.getLocalBounds().width, _HeadlineNameHeight + 30 + i * (TextName.getLocalBounds().height + 20)));

		TextScore.setFont(_Font);
		TextScore.setCharacterSize(25);
		TextScore.setString(std::to_string(_PlayerList[i].Score));
		TextScore.setPosition(_HeadlineScore.getPosition() + sf::Vector2f(_HeadlineScoreWidth - TextScore.getLocalBounds().width, _HeadlineNameHeight + 30 + i * (TextName.getLocalBounds().height + 20)));


		RenderWindow.draw(TextName);
		RenderWindow.draw(TextLevel);
		RenderWindow.draw(TextScore);
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

void Highscore::TestPrintPlayers()
{
	for (unsigned int i = 0; i < _PlayerList.size(); i++)
	{
		std::cout << "Name: " << _PlayerList[i].Name << " Level: " << _PlayerList[i].Level << " Score: " << _PlayerList[i].Score << std::endl;
	}
}

