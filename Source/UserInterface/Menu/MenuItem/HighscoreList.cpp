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
//    _HeadlineNameHeight = _HeadlineName.getLocalBounds().height + 30;

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
                rh::centerText(_HighscoreTexts[i][j]);
            }

            if (i == _FW.getHighscoreManager().getHighlightedRow()) {
                _HighscoreTexts[i][j].setFillColor(sf::Color::Yellow);
            } else {
                _HighscoreTexts[i][j].setFillColor(sf::Color::White);
            }
        }
    }
}
