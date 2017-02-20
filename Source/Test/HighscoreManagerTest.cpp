
#include "stdafx.h"
#include "Test/HighscoreManagerTest.h"
#include "Framework/Framework.h"

HighscoreManagerTest::HighscoreManagerTest() {
    std::cout << "Testing HighscoreManager" << std::endl;
    testSaving();
    testLoading();
}

void HighscoreManagerTest::testSaving() {
    if (rh::file_exists(_FilePath)) {
        std::remove(_FilePath);
    }

    Framework fw;
    HighscoreManager manager(fw);
    std::string name = "Test";
    manager.saveScoreWithName(name, 1, 100.0f);
    manager.saveScoreWithName(name, 1, 100.0f);
    manager.saveScoreWithName(name, 1, 100.0f);
    manager.saveScoreWithName(name, 1, 100.0f);
    manager.saveScoreWithName(name, 1, 100.0f);
    manager.saveScoreTable();

    assertTrue(rh::file_exists(_FilePath));

    std::string line;
    std::ifstream scoreFile;
    scoreFile.open(_FilePath, std::ios::in | std::ofstream::binary);

    if (scoreFile.is_open()) {
        unsigned long length = 0;
        scoreFile.read((char *) &length, sizeof(length));

        assertEqual((int) 5, (int) length);

        for (int i = 0; i < length; i++) {
            PlayerScore newPlayer;
            newPlayer << scoreFile;

            assertEqual(100, newPlayer.getScore());
            assertEqual(1, newPlayer.getLevel());
            assertEqual((std::string) "Test", newPlayer.getName());
        }
        scoreFile.close();
        std::remove(_FilePath);
    } else std::cout << "Unable to open file";
}

void HighscoreManagerTest::testLoading() {
    if (rh::file_exists(_FilePath)) {
        std::remove(_FilePath);
    }

    std::ofstream scoreFile;
    scoreFile.open(_FilePath);

    unsigned long length = 3;
    scoreFile.write((char *) &length, sizeof(length));

    PlayerScore score("Test", 3, 300);
    score.setRank(1);
    score >> scoreFile;

    score = PlayerScore("Test", 2, 200);
    score.setRank(2);
    score >> scoreFile;

    score = PlayerScore("Test", 1, 100);
    score.setRank(3);
    score >> scoreFile;

    scoreFile.close();


    Framework fw;
    HighscoreManager manager(fw);
    manager.loadScoreTable();

    assertEqual((unsigned long) 3, manager.getNumScores());
    for (unsigned long i = 0; i < manager.getNumScores(); i++) {
        score = manager.getScore(i);
        assertEqual((std::string) "Test", score.getName());
        assertEqual((int) (400 - (i + 1) * 100), score.getScore());
        assertEqual((int) (i + 1), score.getRank());
        assertEqual((int) (3 - i), score.getLevel());
    }

    std::remove(_FilePath);
}


