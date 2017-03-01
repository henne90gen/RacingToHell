#pragma once

#include "stdafx.h"
#include <GameObject/Boss/Action/BossAction.h>

class TimerAction : public BossAction {
public:
    TimerAction(BossCar &boss, float time) : BossAction(boss), _Time(time) {}

    void execute();

private:
    float _Time;
    sf::Clock _Timer;
};
