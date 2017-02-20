
#pragma once

#include "stdafx.h"

class Test {
protected:

    template<typename T>
    void assertEqual(T expected, T actual) {
        if (expected != actual) {
            std::cerr << actual << " doesn't equal expected " << expected << std::endl;
            exit(1);
        }
    }

    void assertEqual(std::string expected, std::string actual);

    void assertTrue(bool value);
};
