
#include "Test/BossActionTest.h"
#include <Test/TestBoss.h>
#include <GameObject/Boss/Action/MoveToPosition.h>
#include <Framework/Framework.h>

BossActionTest::BossActionTest() : Test("BossActions") {
    testDriveToDefault();
}

void BossActionTest::testDriveToDefault() {
    sf::Texture texture;
    texture.loadFromFile("Resources/Texture/BossCar/CannonTank.png");
    Framework framework;
    GameObjectManager gom(framework);

    // Setup
    TestBoss boss = TestBoss(texture, gom);
    boss.setPos(sf::Vector2f(0, 0));
    boss.setSpeed(10);
    MoveToPosition firstAction = MoveToPosition(boss, sf::Vector2f(10, 0));

    // First test
    for (int i = 0; i < 11; i++) {
        firstAction.execute();
        boss.update(0.1f);
    }

    assertTrue("First action has not been executed.", firstAction.hasBeenExecuted());
    assertEqual(9.0f, boss.getPosition().x);
    assertEqual(0.0f, boss.getPosition().y);


    // Second test
    boss.setPos(sf::Vector2f(0, 0));
    boss.setSpeed(10);
    MoveToPosition secondAction = MoveToPosition(boss, sf::Vector2f(3, 4));

    for (int i = 0; i < 6; i++) {
        secondAction.execute();
        boss.update(0.1f);
    }

    assertTrue("Second action has not been executed.", secondAction.hasBeenExecuted());
    assertEqual(2.4f, boss.getPosition().x);
    assertEqual(3.2f, boss.getPosition().y);

    // TODO test that boss doesn't overshoot default position
}
