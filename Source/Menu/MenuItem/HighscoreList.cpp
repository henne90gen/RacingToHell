#include "stdafx.h"
#include "Menu/MenuItem\HighscoreList.h"

HighscoreList::HighscoreList(sf::Vector2f Position) : _Gap(42), _Filename("Resources/Data/Highscore.sco"), _NumScores(10)
{
	_Background.setFillColor(sf::Color(0, 0, 0, 100));
	_Background.setSize(sf::Vector2f(450, 530));
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

	loadScoreTable();
}

HighscoreList::~HighscoreList()
{

}

void HighscoreList::render(sf::RenderWindow& RenderWindow)
{
	RenderWindow.draw(_Background);
	RenderWindow.draw(_HeadlineRank);
	RenderWindow.draw(_HeadlineName);
	RenderWindow.draw(_HeadlineLevel);
	RenderWindow.draw(_HeadlineScore);

	for (unsigned int i = 0; i < _PlayerList.size(); i++)
	{
		bool newTopTen = false;
		// Render rank 1 - 10
		if (i < _NumScores) {
			_HighscoreTexts[i].setString(std::to_string(_PlayerList[i].Rank));
			_HighscoreTexts[i].setOrigin(sf::Vector2f(_HighscoreTexts[i].getLocalBounds().width / 2.0f, 0));
			_HighscoreTexts[i].setPosition(_HeadlineRank.getPosition() + sf::Vector2f(_HeadlineRankWidth / 2.0f, _HeadlineNameHeight + i * _Gap));

			_HighscoreTexts[i + _NumScores].setString(_PlayerList[i].Name);
			_HighscoreTexts[i + _NumScores].setPosition(_HeadlineName.getPosition() + sf::Vector2f(0, _HeadlineNameHeight + i * _Gap));

			_HighscoreTexts[i + 2 * _NumScores].setString(std::to_string(_PlayerList[i].Level));
			_HighscoreTexts[i + 2 * _NumScores].setPosition(_HeadlineLevel.getPosition() + sf::Vector2f(_HeadlineLevelWidth - _HighscoreTexts[i + 2 * _NumScores].getLocalBounds().width, _HeadlineNameHeight + i * _Gap));

			_HighscoreTexts[i + 3 * _NumScores].setString(std::to_string(_PlayerList[i].Score));
			_HighscoreTexts[i + 3 * _NumScores].setPosition(_HeadlineScore.getPosition() + sf::Vector2f(_HeadlineScoreWidth - _HighscoreTexts[i + 3 * _NumScores].getLocalBounds().width, _HeadlineNameHeight + i * _Gap));

			if (_PlayerList[i].Score == _CurrentScore && _PlayerList[i].Name == _CurrentName && _PlayerList[i].Level == _CurrentLevel) {
				_HighscoreTexts[i].setColor(sf::Color::Yellow);
				_HighscoreTexts[i + _NumScores].setColor(sf::Color::Yellow);
				_HighscoreTexts[i + 2 * _NumScores].setColor(sf::Color::Yellow);
				_HighscoreTexts[i + 3 * _NumScores].setColor(sf::Color::Yellow);
				newTopTen = true;
			}
			else {
				_HighscoreTexts[i].setColor(sf::Color::White);
				_HighscoreTexts[i + _NumScores].setColor(sf::Color::White);
				_HighscoreTexts[i + 2 * _NumScores].setColor(sf::Color::White);
				_HighscoreTexts[i + 3 * _NumScores].setColor(sf::Color::White);
			}

			RenderWindow.draw(_HighscoreTexts[i]);
			RenderWindow.draw(_HighscoreTexts[i + _NumScores]);
			RenderWindow.draw(_HighscoreTexts[i + 2 * _NumScores]);
			RenderWindow.draw(_HighscoreTexts[i + 3 * _NumScores]);
		}
		// Stop the for loop if player made it into the top 10
		else if (newTopTen) {
			_HighscoreTexts[_NumScores].setString("");
			_HighscoreTexts[_NumScores + _NumScores].setString("");
			_HighscoreTexts[_NumScores + 2 * _NumScores].setString("");
			_HighscoreTexts[_NumScores + 3 * _NumScores].setString("");
			break;
		}
		// Render 11th item that shows the players rank in the whole highscore list
		else if (_PlayerList[i].Score == _CurrentScore && _PlayerList[i].Name == _CurrentName && _PlayerList[i].Level == _CurrentLevel) {
			_HighscoreTexts[_NumScores].setString(std::to_string(_PlayerList[i].Rank));
			_HighscoreTexts[_NumScores].setOrigin(sf::Vector2f(_HighscoreTexts[_NumScores].getLocalBounds().width / 2.0f, 0));
			_HighscoreTexts[_NumScores].setPosition(_HeadlineRank.getPosition() + sf::Vector2f(_HeadlineRankWidth / 2.0f, _HeadlineNameHeight + _NumScores * _Gap));

			_HighscoreTexts[_NumScores + _NumScores].setString(_PlayerList[i].Name);
			_HighscoreTexts[_NumScores + _NumScores].setPosition(_HeadlineName.getPosition() + sf::Vector2f(0, _HeadlineNameHeight + _NumScores * _Gap));

			_HighscoreTexts[_NumScores + 2 * _NumScores].setString(std::to_string(_PlayerList[i].Level));
			_HighscoreTexts[_NumScores + 2 * _NumScores].setPosition(_HeadlineLevel.getPosition() + sf::Vector2f(_HeadlineLevelWidth - _HighscoreTexts[_NumScores + 2 * _NumScores].getLocalBounds().width, _HeadlineNameHeight + _NumScores * _Gap));

			_HighscoreTexts[_NumScores + 3 * _NumScores].setString(std::to_string(_PlayerList[i].Score));
			_HighscoreTexts[_NumScores + 3 * _NumScores].setPosition(_HeadlineScore.getPosition() + sf::Vector2f(_HeadlineScoreWidth - _HighscoreTexts[_NumScores + 3 * _NumScores].getLocalBounds().width, _HeadlineNameHeight + _NumScores * _Gap));

			_HighscoreTexts[_NumScores].setColor(sf::Color::Yellow);
			_HighscoreTexts[_NumScores + _NumScores].setColor(sf::Color::Yellow);
			_HighscoreTexts[_NumScores + 2 * _NumScores].setColor(sf::Color::Yellow);
			_HighscoreTexts[_NumScores + 3 * _NumScores].setColor(sf::Color::Yellow);

			RenderWindow.draw(_HighscoreTexts[_NumScores]);
			RenderWindow.draw(_HighscoreTexts[_NumScores + _NumScores]);
			RenderWindow.draw(_HighscoreTexts[_NumScores + 2 * _NumScores]);
			RenderWindow.draw(_HighscoreTexts[_NumScores + 3 * _NumScores]);
			break;
		}
	}
}

void HighscoreList::loadScoreTable()
{
	std::ifstream FileStream;
	FileStream.open(_Filename, std::ios::in | std::ifstream::binary);
	int length;
	FileStream.read((char*)&length, sizeof(length));

	_PlayerList.clear();
	for (int i = 0; i < length; i++) {
		Player newPlayer;
		newPlayer << FileStream;
		_PlayerList.push_back(newPlayer);
	}

	FileStream.close();

	SortScoreTable();
}

void HighscoreList::SaveScoreTable()
{
	std::ofstream FileStream;
	FileStream.open(_Filename, std::ios::out | std::ofstream::binary);
	int length = _PlayerList.size();
	FileStream.write((char*)&length, sizeof(length));
	for (unsigned int i = 0; i < _PlayerList.size(); i++)
	{
		_PlayerList[i] >> FileStream;
	}
	FileStream.close();
}

void HighscoreList::SortScoreTable()
{
	std::sort(_PlayerList.rbegin(), _PlayerList.rend());
	for (int i = 0; i < _PlayerList.size(); i++) {
		_PlayerList[i].Rank = i + 1;
	}

	_HighscoreTexts.clear();
	for (unsigned int i = 0; i < 11 * 4; i++)
	{
		sf::Text HighscoreText;
		HighscoreText.setFont(_Font);
		HighscoreText.setCharacterSize(25);

		_HighscoreTexts.push_back(HighscoreText);
	}

	SaveScoreTable();
}

void HighscoreList::PlacePlayer(std::string& Name, int Level)
{
	_CurrentName = Name;
	_CurrentLevel = Level;
	
	Player newPlayer;
	newPlayer.Name = _CurrentName;
	newPlayer.Level = _CurrentLevel;
	newPlayer.Score = _CurrentScore;

	_PlayerList.push_back(newPlayer);

	SortScoreTable();
}
