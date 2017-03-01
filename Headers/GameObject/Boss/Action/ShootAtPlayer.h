#pragma once

#include "stdafx.h"
#include <GameObject/Boss/Action/BossAction.h>

class ShootAtPlayer : public BossAction {
public:
    ShootAtPlayer(BossCar &boss, std::shared_ptr<BossAction> parentAction, std::vector<float> shotFrequencies, std::vector<float> salveAngles);

    void execute();

private:
    sf::Clock _Timer;
    unsigned int _CurrentFrequencyIndex = 0;
    std::vector<float> _ShotFrequencies;
    std::vector<float> _SalveAngles;
    std::shared_ptr<BossAction> _ParentAction;
};
