
#include "Test/BossActionTest.h"
#include <Test/TestBoss.h>
#include <GameObject/Boss/Action/BossAction.h>
#include <GameObject/Boss/Action/DriveToDefault.h>

BossActionTest::BossActionTest() : Test("BossActions") {
    testDriveToDefault();
}

void BossActionTest::testDriveToDefault() {
    sf::Texture texture;
    texture.loadFromFile("Resources/Texture/BossCar/CannonTank.png");

    // Setup
    TestBoss boss = TestBoss(texture);
    boss.setPos(sf::Vector2f(0, 0));
    boss.setDefaultPosition(sf::Vector2f(10, 0));
    boss.setSpeed(10);
    DriveToDefault action = DriveToDefault(boss);

    // First test
    for (int i = 0; i < 10; i++) {
        action.execute();
        boss.update(0.1f);
    }

    assertTrue(action.hasBeenExecuted());
    assertEqual(10.0f, boss.getPosition().x);
    assertEqual(0.0f, boss.getPosition().y);

    // Second test
    boss.setPos(sf::Vector2f(0, 0));
    boss.setDefaultPosition(sf::Vector2f(3, 4));
    boss.setSpeed(10);

    for (int i = 0; i < 5; i++) {
        action.execute();
        boss.update(0.1f);
    }

    assertTrue(action.hasBeenExecuted());
    assertEqual(3.0f, boss.getPosition().x);
    assertEqual(4.0f, boss.getPosition().y);

    // TODO test that boss doesn't overshoot default position
}
