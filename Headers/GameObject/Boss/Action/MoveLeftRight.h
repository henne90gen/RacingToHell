#pragma once

#include "stdafx.h"
#include <GameObject/Boss/Action/BossAction.h>

class MoveLeftRight : public BossAction {
public:
    MoveLeftRight(BossCar &boss, sf::Vector2f leftPos, sf::Vector2f rightPos, unsigned int numSwitches);

    void execute();

private:
    unsigned int _NumSwitches;
    sf::Vector2f _LastTargetPosition;
    sf::Vector2f _LeftTargetPosition;
    sf::Vector2f _RightTargetPosition;
};

