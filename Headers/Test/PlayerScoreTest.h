
#pragma once

#include <Test/Test.h>

class PlayerScoreTest : public Test {
public:

    PlayerScoreTest();

    void testSaving();

    void testLoading();

private:
    const char *_SavingPath = "playerScoreSavingTest";
    const char *_LoadingPath = "playerScoreLoadingTest";
};

