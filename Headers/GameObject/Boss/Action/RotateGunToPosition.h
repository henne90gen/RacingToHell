#pragma once

#include "stdafx.h"
#include <GameObject/Boss/Action/BossAction.h>

class RotateGunToPosition : public BossAction {
public:
    RotateGunToPosition(BossCar &boss, sf::Vector2f finalPosition);

    void execute();

private:
    sf::Vector2f _FinalPosition;
};

