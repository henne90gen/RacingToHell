#include "stdafx.h"
#include <Test/HelperTest.h>

HelperTest::HelperTest() : Test("Helpers") {
    testLength();
    testNormalize();
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

    test = sf::Vector2f(0,5);
    assertEqual(1.0f, rh::vectorLength(rh::normalize(test)));

    test = sf::Vector2f(5,0);
    assertEqual(1.0f, rh::vectorLength(rh::normalize(test)));
}
