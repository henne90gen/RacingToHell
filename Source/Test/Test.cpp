#include "Test/Test.h"

Test::Test(std::string name) {
    std::cout << "Testing " << name << std::endl;
}

void Test::assertEqual(std::string msg, std::string expected, std::string actual) {
    if (expected != actual) {
        std::cerr << msg << std::endl;
        exit(1);
    }
}

void Test::assertEqual(std::string expected, std::string actual) {
    assertEqual("Expected '" + expected + "', got '" + actual + "' instead.", expected, actual);
}

void Test::assertTrue(std::string msg, bool value) {
    if (!value) {
        std::cerr << msg << std::endl;
        exit(1);
    }
}

void Test::assertTrue(bool value) {
    assertTrue("Not true!", value);
}

void Test::assertFalse(bool value) {
    assertFalse("Not false!", value);
}

void Test::assertFalse(std::string msg, bool value) {
    if (!value) {
        std::cerr << msg << std::endl;
        exit(1);
    }
}
