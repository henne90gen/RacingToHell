//
// Created by henne on 27.11.16.
//

#pragma once

class Framework;

class Updater {
public:

    Updater(Framework &framework);

    void update(float frameTime);

private:
    Framework &_FW;
};
