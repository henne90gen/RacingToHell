#pragma once

#include <GameObject/Boss/Action/BossAction.h>

class Spin : public BossAction {
public:
    Spin(BossCar &boss);

    void execute();
};
