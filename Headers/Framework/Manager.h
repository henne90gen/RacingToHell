#pragma once

class Framework;

class Manager {
public:
    Manager(Framework &framework) : _FW(framework) {}

    virtual ~Manager() {}

protected:
    Framework &_FW;
};


