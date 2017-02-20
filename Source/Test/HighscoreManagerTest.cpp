
#include "stdafx.h"
#include "Test/HighscoreManagerTest.h"
#include "Framework/Framework.h"

HighscoreManagerTest::HighscoreManagerTest() {
    std::cout << "Testing HighscoreManager" << std::endl;
    testSaving();
    testLoading();
}

void HighscoreManagerTest::testSaving() {
    Framework fw;
    HighscoreManager manager(fw);
    std::string name = "Test";
    manager.saveScoreWithName(name, 1, 100.0f);
    manager.saveScoreTable();

    const char *filePath = "Resources/scores.sco";
    assertTrue(rh::file_exists(filePath));

    std::string line;
    std::ifstream scoreFile;
    scoreFile.open(filePath, std::ios::in | std::ofstream::binary);

    if (scoreFile.is_open()) {
        unsigned long length = 0;
        scoreFile.read((char *) &length, sizeof(length));

        assertEqual((int) 1, (int) length);

        for (int i = 0; i < length; i++) {
            PlayerScore newPlayer;
            newPlayer << scoreFile;

            assertEqual(100, newPlayer.getScore());
            assertEqual(1, newPlayer.getLevel());
            assertEqual((std::string) "Test", newPlayer.getName());
        }
        scoreFile.close();
        std::remove(filePath);
    } else std::cout << "Unable to open file";
}

void HighscoreManagerTest::testLoading() {

}


