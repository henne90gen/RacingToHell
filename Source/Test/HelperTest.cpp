#include "stdafx.h"
#include <Test/HelperTest.h>

HelperTest::HelperTest() : Test("Helpers") {
    testRound();
    testLength();
    testNormalize();
    testVectorFromAngle();
}

void HelperTest::testRound() {
    assertEqual(1.2f, rh::round(1.23456789f, 1));
    assertEqual(1.23f, rh::round(1.23456789f, 2));
    assertEqual(1.234f, rh::round(1.23456789f, 3));
}

void HelperTest::testLength() {
    sf::Vector2f test(3, 4);
    assertEqual(5.0f, rh::vectorLength(test));

    test = sf::Vector2f(0, 5);
    assertEqual(5.0f, rh::vectorLength(test));

    test = sf::Vector2f(5, 0);
    assertEqual(5.0f, rh::vectorLength(test));
}

void HelperTest::testNormalize() {
    sf::Vector2f test(3, 4);
    assertEqual(1.0f, rh::vectorLength(rh::normalize(test)));

    test = sf::Vector2f(0, 5);
    assertEqual(1.0f, rh::vectorLength(rh::normalize(test)));

    test = sf::Vector2f(5, 0);
    assertEqual(1.0f, rh::vectorLength(rh::normalize(test)));
}

void HelperTest::testVectorFromAngle() {
    float angle = 0;
    assertEqual(sf::Vector2f(1, 0), rh::getVectorFromAngle(angle));

    angle = PI;
    assertEqual(sf::Vector2f(-1, 0), rh::getVectorFromAngle(angle));

    angle = PI / 2;
    assertEqual(sf::Vector2f(0, 1), rh::getVectorFromAngle(angle));

    angle = PI * 3 / 2;
    assertEqual(sf::Vector2f(0, -1), rh::getVectorFromAngle(angle));
}
