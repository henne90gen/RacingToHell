#include "stdafx.h"
#include "Framework/NetworkManager.h"

NetworkManager::NetworkManager(Framework &fw) : Manager(fw), _NetworkThread(&NetworkManager::run, this) {}

void NetworkManager::run() {
    std::cout << "Started network thread" << std::endl;
}

void NetworkManager::startThread() {
    _NetworkThread.launch();
}
