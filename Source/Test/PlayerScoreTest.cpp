
#include "stdafx.h"
#include <Framework/PlayerScore.h>
#include "Test/PlayerScoreTest.h"

PlayerScoreTest::PlayerScoreTest() {
    std::cout << "Testing PlayerScore" << std::endl;

    testSaving();
    testLoading();

}

void PlayerScoreTest::testSaving() {
    if (rh::file_exists(_SavingPath)) {
        std::remove(_SavingPath);
    }

    std::ofstream fileStream;
    fileStream.open(_SavingPath);

    PlayerScore score("Test", 2, 100);
    score.setRank(1);
    score >> fileStream;
    fileStream.close();


    std::ifstream ifileStream;
    ifileStream.open(_SavingPath);

    int rank = 0;
    unsigned long nameLength = 0;
    std::string name = "";
    int level = 0;
    int s = 0;

    ifileStream.read((char *) &rank, sizeof(rank));
    ifileStream.read((char *) &nameLength, sizeof(nameLength));

    char buffer[nameLength];
    ifileStream.read(buffer, nameLength);

    ifileStream.read((char *) &level, sizeof(level));
    ifileStream.read((char *) &s, sizeof(s));

    ifileStream.close();

    assertEqual(1, rank);
    assertEqual("Test", buffer);
    assertEqual(2, level);
    assertEqual(100, s);

    std::remove(_SavingPath);
}

void PlayerScoreTest::testLoading() {
    if (rh::file_exists(_LoadingPath)) {
        std::remove(_LoadingPath);
    }

    std::ofstream ofileStream;
    ofileStream.open(_LoadingPath);

    int rank = 1;
    int level = 2;
    int s = 100;

    ofileStream.write((char *) &rank, sizeof(rank));
    unsigned long length = 4;
    ofileStream.write((char *) &length, sizeof(length));
    ofileStream << "Test";
    ofileStream.write((char *) &level, sizeof(level));
    ofileStream.write((char *) &s, sizeof(s));

    ofileStream.close();

    std::ifstream fileStream;
    fileStream.open(_LoadingPath);

    PlayerScore score;
    score << fileStream;

    assertEqual(rank, score.getRank());
    assertEqual((std::string) "Test", score.getName());
    assertEqual(s, score.getScore());
    assertEqual(level, score.getLevel());

    fileStream.close();
    std::remove(_LoadingPath);
}
