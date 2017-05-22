
#include <Test/HelperTest.h>
#include <Test/PlayerScoreTest.h>
#include <Test/HighscoreManagerTest.h>
#include <Test/BossActionTest.h>

int main() {
    HelperTest();
    PlayerScoreTest();
    HighscoreManagerTest();
    BossActionTest();
    std::cout << "Tests complete" << std::endl;
    return 0;
}
