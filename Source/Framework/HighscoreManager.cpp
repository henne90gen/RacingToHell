//
// Created by henne on 24.01.17.
//

#include "stdafx.h"
#include "Framework/HighscoreManager.h"
#include "Framework/Framework.h"

HighscoreManager::HighscoreManager(Framework &framework) : _FW(framework), _Filename("Resources/Data/Highscore.sco"),
                                                           _HighlightedRow(0) {
    // FIXME remove after testing
    for (int i = 0; i < 11; i++) {
        PlayerScore newPlayer("Hello" + std::to_string(i), i, i);
        _ScoreList.push_back(newPlayer);
    }

    sortScoreTable();
    // FIXME remove after testing ^^^^^^

    // FIXME uncomment after testing
//    loadScoreTable();
}

void HighscoreManager::loadScoreTable() {
    std::ifstream fileStream;
    fileStream.open(_Filename, std::ios::in | std::ifstream::binary);
    int length = 0;
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
    fileStream.open(_Filename, std::ios::out | std::ofstream::binary);
    unsigned long length = _ScoreList.size();
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

    PlayerScore newPlayer(name, _FW.getLevelManager().getLevel(), _FW.getLevelManager().getScore());
    _CurrentPlayerScore = newPlayer;
    _ScoreList.push_back(newPlayer);

    sortScoreTable();
}
