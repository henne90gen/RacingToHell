#pragma once

#include <GameObject/Boss/BossCar.h>
#include "BossAction.h"

class DriveToDefault : public BossAction {
public:
    DriveToDefault(BossCar &boss) : BossAction(), _Boss(boss) {}

    virtual void execute();

private:
    BossCar &_Boss;
};
