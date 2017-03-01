#pragma once

#include "stdafx.h"

class Test {
protected:

    Test() {}

    Test(std::string name);

    template<typename T>
    void assertEqual(std::string msg, T expected, T actual) {
        if (expected != actual) {
            std::cerr << msg << std::endl;
            exit(1);
        }
    }

    template<typename T>
    void assertEqual(T expected, T actual) {
        std::string msg = "Expected '";
        msg += rh::to_string(expected);
        msg += "', got '";
        msg += rh::to_string(actual);
        msg += "' instead.";
        assertEqual(msg, expected, actual);
    }

    void assertEqual(std::string msg, std::string expected, std::string actual);

    void assertEqual(std::string expected, std::string actual);

    void assertTrue(std::string msg, bool value);

    void assertTrue(bool value);

    void assertFalse(std::string msg, bool value);

    void assertFalse(bool value);
};
