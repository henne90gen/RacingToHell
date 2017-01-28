#pragma once

#include <fstream>
#include <sstream>
#include <algorithm>

#include "Serializable.h"
#include "MenuItem.h"
#include "Framework/HighscoreManager.h"

class Framework;

class HighscoreList {
public:

    /*
        Prints out the ranks 1-10 of the highscore list.
        If the new highscore didn't make the top 10 it will be displayed below with the corresponding rank
        @param pos Position of the HighscoreList on screen
    */
    HighscoreList(Framework &framework, sf::Vector2f pos);

    ~HighscoreList() {}

    /*
        Renders the HighscoreList to the specified RenderWindow
        @param window Window to draw to
    */
    void render(sf::RenderWindow &renderWindow);

    void update(float frameTime);
private:
    Framework &_FW;

    sf::RectangleShape _Background;
    sf::Text _HeadlineRank;
    sf::Text _HeadlineName;
    sf::Text _HeadlineLevel;
    sf::Text _HeadlineScore;

    float _HeadlineRankWidth, _HeadlineNameHeight, _HeadlineLevelWidth, _HeadlineScoreWidth, _Gap;

    std::string _CurrentName;

    int _NumScores;

    std::vector<std::vector<sf::Text>> _HighscoreTexts;

    void centerText(sf::Text &text);
};
