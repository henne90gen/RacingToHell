#pragma once

#include <SFML/System/Thread.hpp>
#include "Manager.h"

class NetworkManager : public Manager {
public:
    NetworkManager(Framework &framework);

    void startThread();

private:
    sf::Thread _NetworkThread;

    void run();
};


