#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/HighscoreList.h"
#include <Framework/Framework.h>

HighscoreList::HighscoreList(Framework &framework, sf::Vector2f pos) : _FW(framework), _Gap(42), _NumScores(10) {

    _Background.setFillColor(sf::Color(0, 0, 0, 100));
    _Background.setSize(sf::Vector2f(450, 530));
    _Background.setPosition(pos);

    pos += sf::Vector2f(15, 15);
    float startX = pos.x;
    float startY = pos.y;

    sf::Font &font = _FW.getOptionsManager().getFont();

    _HeadlineRank.setFont(font);
    _HeadlineRank.setCharacterSize(30);
    _HeadlineRank.setStyle(sf::Text::Style::Bold);
    _HeadlineRank.setPosition(pos);
    _HeadlineRank.setString("Rank");
    _HeadlineRankWidth = _HeadlineRank.getLocalBounds().width;

    pos += sf::Vector2f(_HeadlineRank.getLocalBounds().width + 10, 0);

    _HeadlineName = _HeadlineRank;
    _HeadlineName.setPosition(pos);
    _HeadlineName.setString("Name");
    _HeadlineNameHeight = _HeadlineName.getLocalBounds().height + 30;

    pos += sf::Vector2f(_HeadlineName.getLocalBounds().width + 70, 0);

    _HeadlineLevel = _HeadlineRank;
    _HeadlineLevel.setString("Level");
    _HeadlineLevel.setPosition(pos);
    _HeadlineLevelWidth = _HeadlineLevel.getLocalBounds().width;

    pos += sf::Vector2f(_HeadlineLevel.getLocalBounds().width + 30, 0);

    _HeadlineScore = _HeadlineRank;
    _HeadlineScore.setPosition(pos);
    _HeadlineScore.setString("Score");
    _HeadlineScoreWidth = _HeadlineScore.getLocalBounds().width;


    float startingXs[] = {startX + _HeadlineRankWidth / 2.0f, _HeadlineName.getPosition().x,
                          _HeadlineLevel.getPosition().x + _HeadlineLevelWidth / 2.0f,
                          _HeadlineScore.getPosition().x + _HeadlineScoreWidth / 2.0f};

    for (int i = 0; i < 11; i++) {
        pos = sf::Vector2f(startX, startY + (i + 1) * _Gap);
        _HighscoreTexts.push_back(std::vector<sf::Text>());
        for (int j = 0; j < 4; j++) {
            sf::Text text = sf::Text();

            text.setPosition(startingXs[j], pos.y);
            text.setFont(font);
            text.setCharacterSize(30);
            text.setString("test");
            if (j != 1) {
                centerText(text);
            }
            _HighscoreTexts[i].push_back(text);
        }
    }
}

void HighscoreList::render(sf::RenderWindow &renderWindow) {
    renderWindow.draw(_Background);
    renderWindow.draw(_HeadlineRank);
    renderWindow.draw(_HeadlineName);
    renderWindow.draw(_HeadlineLevel);
    renderWindow.draw(_HeadlineScore);

    for (unsigned long i = 0; i < _HighscoreTexts.size(); i++) {
        for (unsigned int j = 0; j < _HighscoreTexts[i].size(); j++) {
            renderWindow.draw(_HighscoreTexts[i][j]);
        }
    }

/*  std::vector<Player> playerList = _FW.getHighscoreManager().getPlayerList();
    for (unsigned int i = 0; i < _PlayerList.size(); i++) {
        bool newTopTen = false;
        // Render rank 1 - 10
        if (i < _NumScores) {
            _HighscoreTexts[i].setString(std::to_string(_PlayerList[i].Rank));
            _HighscoreTexts[i].setOrigin(sf::Vector2f(_HighscoreTexts[i].getLocalBounds().width / 2.0f, 0));
            _HighscoreTexts[i].setPosition(_HeadlineRank.getPosition() +
                                           sf::Vector2f(_HeadlineRankWidth / 2.0f, _HeadlineNameHeight + i * _Gap));

            _HighscoreTexts[i + _NumScores].setString(_PlayerList[i].Name);
            _HighscoreTexts[i + _NumScores].setPosition(
                    _HeadlineName.getPosition() + sf::Vector2f(0, _HeadlineNameHeight + i * _Gap));

            _HighscoreTexts[i + 2 * _NumScores].setString(std::to_string(_PlayerList[i].Level));
            _HighscoreTexts[i + 2 * _NumScores].setPosition(_HeadlineLevel.getPosition() + sf::Vector2f(
                    _HeadlineLevelWidth - _HighscoreTexts[i + 2 * _NumScores].getLocalBounds().width,
                    _HeadlineNameHeight + i * _Gap));

            _HighscoreTexts[i + 3 * _NumScores].setString(std::to_string(_PlayerList[i].Score));
            _HighscoreTexts[i + 3 * _NumScores].setPosition(_HeadlineScore.getPosition() + sf::Vector2f(
                    _HeadlineScoreWidth - _HighscoreTexts[i + 3 * _NumScores].getLocalBounds().width,
                    _HeadlineNameHeight + i * _Gap));

            if (_PlayerList[i].Score == _CurrentScore && _PlayerList[i].Name == _CurrentName &&
                _PlayerList[i].Level == _CurrentLevel) {
                _HighscoreTexts[i].setFillColor(sf::Color::Yellow);
                _HighscoreTexts[i + _NumScores].setFillColor(sf::Color::Yellow);
                _HighscoreTexts[i + 2 * _NumScores].setFillColor(sf::Color::Yellow);
                _HighscoreTexts[i + 3 * _NumScores].setFillColor(sf::Color::Yellow);
                newTopTen = true;
            } else {
                _HighscoreTexts[i].setFillColor(sf::Color::White);
                _HighscoreTexts[i + _NumScores].setFillColor(sf::Color::White);
                _HighscoreTexts[i + 2 * _NumScores].setFillColor(sf::Color::White);
                _HighscoreTexts[i + 3 * _NumScores].setFillColor(sf::Color::White);
            }

            renderWindow.draw(_HighscoreTexts[i]);
            renderWindow.draw(_HighscoreTexts[i + _NumScores]);
            renderWindow.draw(_HighscoreTexts[i + 2 * _NumScores]);
            renderWindow.draw(_HighscoreTexts[i + 3 * _NumScores]);
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
        else if (_PlayerList[i].Score == _CurrentScore && _PlayerList[i].Name == _CurrentName &&
                 _PlayerList[i].Level == _CurrentLevel) {
            _HighscoreTexts[_NumScores].setString(std::to_string(_PlayerList[i].Rank));
            _HighscoreTexts[_NumScores].setOrigin(
                    sf::Vector2f(_HighscoreTexts[_NumScores].getLocalBounds().width / 2.0f, 0));
            _HighscoreTexts[_NumScores].setPosition(_HeadlineRank.getPosition() +
                                                    sf::Vector2f(_HeadlineRankWidth / 2.0f,
                                                                 _HeadlineNameHeight + _NumScores * _Gap));

            _HighscoreTexts[_NumScores + _NumScores].setString(_PlayerList[i].Name);
            _HighscoreTexts[_NumScores + _NumScores].setPosition(
                    _HeadlineName.getPosition() + sf::Vector2f(0, _HeadlineNameHeight + _NumScores * _Gap));

            _HighscoreTexts[_NumScores + 2 * _NumScores].setString(std::to_string(_PlayerList[i].Level));
            _HighscoreTexts[_NumScores + 2 * _NumScores].setPosition(_HeadlineLevel.getPosition() + sf::Vector2f(
                    _HeadlineLevelWidth - _HighscoreTexts[_NumScores + 2 * _NumScores].getLocalBounds().width,
                    _HeadlineNameHeight + _NumScores * _Gap));

            _HighscoreTexts[_NumScores + 3 * _NumScores].setString(std::to_string(_PlayerList[i].Score));
            _HighscoreTexts[_NumScores + 3 * _NumScores].setPosition(_HeadlineScore.getPosition() + sf::Vector2f(
                    _HeadlineScoreWidth - _HighscoreTexts[_NumScores + 3 * _NumScores].getLocalBounds().width,
                    _HeadlineNameHeight + _NumScores * _Gap));

            _HighscoreTexts[_NumScores].setFillColor(sf::Color::Yellow);
            _HighscoreTexts[_NumScores + _NumScores].setFillColor(sf::Color::Yellow);
            _HighscoreTexts[_NumScores + 2 * _NumScores].setFillColor(sf::Color::Yellow);
            _HighscoreTexts[_NumScores + 3 * _NumScores].setFillColor(sf::Color::Yellow);

            renderWindow.draw(_HighscoreTexts[_NumScores]);
            renderWindow.draw(_HighscoreTexts[_NumScores + _NumScores]);
            renderWindow.draw(_HighscoreTexts[_NumScores + 2 * _NumScores]);
            renderWindow.draw(_HighscoreTexts[_NumScores + 3 * _NumScores]);
            break;
        }
    }*/
}

void HighscoreList::update(float frameTime) {
    if (_FW.getHighscoreManager().shouldShowAdditionalLine()) {
        _Background.setSize(sf::Vector2f(450, 530));
    } else {
        _Background.setSize(sf::Vector2f(450, 490));
    }
    for (unsigned long i = 0; i < _HighscoreTexts.size(); i++) {
        if (i >= _FW.getHighscoreManager().getNumScores()) {
            break;
        }
        PlayerScore score = _FW.getHighscoreManager().getScore(i);

        std::vector<std::string> values;
        values.push_back(std::to_string(score.getRank()));
        values.push_back(score.getName());
        values.push_back(std::to_string(score.getLevel()));
        values.push_back(std::to_string(score.getScore()));

        for (int j = 0; j < values.size(); j++) {
            if (!_FW.getHighscoreManager().shouldShowAdditionalLine() && i == _HighscoreTexts.size() - 1) {
                _HighscoreTexts[i][j].setString("");
                continue;
            } else {
                _HighscoreTexts[i][j].setString(values[j]);
            }

            if (j != 1) {
                centerText(_HighscoreTexts[i][j]);
            }

            if (i == _FW.getHighscoreManager().getHighlightedRow()) {
                _HighscoreTexts[i][j].setFillColor(sf::Color::Yellow);
            } else {
                _HighscoreTexts[i][j].setFillColor(sf::Color::White);
            }
        }
    }
}

void HighscoreList::centerText(sf::Text &text) {
    text.setOrigin(text.getLocalBounds().width / 2, 0);
}
