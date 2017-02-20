#include "Test/Test.h"

void Test::assertTrue(bool value) {
    if (!value) {
        std::cerr << "Not true!" << std::endl;
        exit(1);
    }
}

void Test::assertEqual(std::string expected, std::string actual) {
    if (expected != actual) {
        std::cerr << "Expected '" << expected << "', got '" << actual << "' instead." << std::endl;
        exit(1);
    }
}
