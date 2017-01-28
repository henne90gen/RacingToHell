//
// Created by henne on 24.01.17.
//

#pragma once

#include "Serializable.h"
#include "Framework/PlayerScore.h"

class Framework;

class HighscoreManager {
public:

    HighscoreManager(Framework &framework);

    PlayerScore getScore(unsigned long index) { return _ScoreList.at(index); }

    /**
     * Loads the highscore list from file
     */
    void loadScoreTable();

    /**
     * Saves the highscore list to file
     */
    void saveScoreTable();

    /**
     * Adds the player of the last round to the HighscoreList
     * @param name Name of the player
     * @param level Level the player reached
     */
    void saveScoreWithName(std::string &name);

    unsigned long getNumScores() {
        return _ScoreList.size();
    }

    bool shouldShowAdditionalLine() {
        return _ShowAdditionalLine;
    }

    void setShowAdditionalLine(bool addLine) { _ShowAdditionalLine = addLine; }

    int getHighlightedRow() {
        if (!shouldShowAdditionalLine()) {
            return 10;
        }
        return _HighlightedRow;
    }

    void setHighlightedRow(int row) { _HighlightedRow = row; }

private:
    Framework &_FW;

    std::vector<PlayerScore> _ScoreList;

    std::string _Filename;

    PlayerScore _CurrentPlayerScore;

    int _HighlightedRow;

    bool _ShowAdditionalLine;

    /**
     * Sorts the player in the score table by score
     */
    void sortScoreTable();
};


