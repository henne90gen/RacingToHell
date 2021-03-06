//
// Created by henne on 24.01.17.
//

#include "stdafx.h"
#include "Framework/HighscoreManager.h"
#include "Framework/Framework.h"

HighscoreManager::HighscoreManager(Framework &framework) : Manager(framework), _HighlightedRow(0) {}

void HighscoreManager::loadScoreTable() {
    if (!rh::file_exists(_ScorePath)) {
        // TODO create scores file with sample scores
        return;
    }

    std::ifstream fileStream;
    fileStream.open(_ScorePath, std::ios::in);
    unsigned long length = 0;
    fileStream.read((char *) &length, sizeof(length));
    _ScoreList.clear();
    for (int i = 0; i < length; i++) {
        PlayerScore newPlayer;
        newPlayer << fileStream;
        _ScoreList.push_back(newPlayer);
    }
    fileStream.close();

    sortScoreTable();
}

void HighscoreManager::saveScoreTable() {
    std::ofstream fileStream;
    fileStream.open(_ScorePath, std::ios::out | std::ofstream::binary);
    unsigned long length = (unsigned long) _ScoreList.size();
    fileStream.write((char *) &length, sizeof(length));
    for (unsigned int i = 0; i < _ScoreList.size(); i++) {
        _ScoreList[i] >> fileStream;
    }
    fileStream.close();
}

void HighscoreManager::sortScoreTable() {
    std::sort(_ScoreList.rbegin(), _ScoreList.rend());
    for (int i = 0; i < _ScoreList.size(); i++) {
        _ScoreList[i].setRank(i + 1);
    }

    saveScoreTable();
}

void HighscoreManager::saveScoreWithName(std::string &name) {
   saveScoreWithName(name, _FW.getLevelManager().getLevel(), _FW.getLevelManager().getScore());
}

void HighscoreManager::saveScoreWithName(std::string &name, int level, float score) {
    PlayerScore newPlayer(name, level, (int) score);
    _CurrentPlayerScore = newPlayer;
    _ScoreList.push_back(newPlayer);

    sortScoreTable();
}
