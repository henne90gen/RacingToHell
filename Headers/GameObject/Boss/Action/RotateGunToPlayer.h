#pragma once

#include "stdafx.h"
#include "GameObject/Boss/Action/BossAction.h"

class RotateGunToPlayer : public BossAction {
public:
    /**
     * This will keep the gun rotating to point at the player until the parent action has finished executing.
     */
    RotateGunToPlayer(BossCar &boss, std::shared_ptr<BossAction> parentAction);

    void execute();
};

