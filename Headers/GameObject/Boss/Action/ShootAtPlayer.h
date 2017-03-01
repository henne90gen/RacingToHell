#pragma once

#include <GameObject/Boss/Action/BossAction.h>

class ShootAtPlayer : public BossAction {
public:
    ShootAtPlayer(BossCar &boss) : BossAction(boss) {}

    void execute();
};
