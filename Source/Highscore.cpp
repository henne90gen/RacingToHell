#include "stdafx.h"
#include "Highscore.h"

Highscore::Highscore(sf::Vector2f Position)
{
	_Font.loadFromFile("Resources\\Font\\arial.ttf");
	_Background.setFillColor(sf::Color(0, 0, 0, 100));
	_Background.setSize(sf::Vector2f(450, 500));
	_Background.setPosition(Position);

	_Filename = "Resources\\Data\\Highscore.txt";

	//Test
	Player Player1;
	Player1.Name = "Testperson1";
	Player1.Level = 4;
	Player1.Score = 12400;

	Player Player2;
	Player2.Name = "Testperson2";
	Player2.Level = 3;
	Player2.Score = 6500;

	_PlayerList.push_back(Player2);
	_PlayerList.push_back(Player1);

	SortScoreTable();
	SaveScoreTable();
}

Highscore::~Highscore()
{

}

void Highscore::render(sf::RenderWindow& RenderWindow)
{
	RenderWindow.draw(_Background);
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