#include "stdafx.h"
#include <Test/HelperTest.h>

HelperTest::HelperTest() : Test("Helpers") {
    testRound();
    testLength();
    testNormalize();
    testVectorFromAngle();
    testRotateVector();
    testAngleBetweenVectors();
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
    assertEqual(sf::Vector2f(1, 0), rh::vectorFromAngle(angle));

    angle = PI;
    assertEqual(sf::Vector2f(-1, 0), rh::vectorFromAngle(angle));

    angle = PI / 2;
    assertEqual(sf::Vector2f(0, 1), rh::vectorFromAngle(angle));

    angle = PI * 3 / 2;
    assertEqual(sf::Vector2f(0, -1), rh::vectorFromAngle(angle));

    angle = PI / 4;
    assertEqual(sf::Vector2f(0.70710678118f, 0.70710678118f), rh::vectorFromAngle(angle));
}

void HelperTest::testRotateVector() {
    sf::Vector2f vec(1, 0);
    float angle = PI / 2;
    assertEqual(sf::Vector2f(0, 1), rh::rotateVector(vec, angle));
    assertEqual(sf::Vector2f(0, -1), rh::rotateVector(vec, -angle));
}

void HelperTest::testAngleBetweenVectors() {
    sf::Vector2f vec1(1, 0);
    sf::Vector2f vec2(0, 1);

    assertEqual(PI / 2, rh::angleBetweenVectors(vec1, vec2));

    vec1 = sf::Vector2f(0.38268343236f, 0.92387953251f); // 3 * PI / 8
    vec2 = sf::Vector2f(0.92387953251f, 0.38268343236f); //     PI / 8
    assertEqual(-PI / 4, rh::angleBetweenVectors(vec1, vec2));
    assertEqual(PI / 4, rh::angleBetweenVectors(vec2, vec1));
}
