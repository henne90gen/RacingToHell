#pragma once

#include "Framework/Manager.h"
#include "Serializable.h"
#include "Framework/PlayerScore.h"

class HighscoreManager : public Manager {
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

    void saveScoreWithName(std::string &name, int level, float score);

    /**
     * @return Amount of saved scores
     */
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
    std::vector<PlayerScore> _ScoreList;

    const std::string _ScorePath = "Resources/scores.sco";

    PlayerScore _CurrentPlayerScore;

    int _HighlightedRow;

    bool _ShowAdditionalLine;

    /**
     * Sorts the player in the score table by score
     */
    void sortScoreTable();
};


