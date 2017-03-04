#pragma once

#include "stdafx.h"
#include "GameObject/Boss/Action/BossAction.h"

class PointGunAtPlayer : public BossAction {
public:
    /**
     * This will keep the gun moving to point at the player until parent action has finished executing.
     */
    PointGunAtPlayer(BossCar &boss, std::shared_ptr<BossAction> parentAction);

    void execute();
};

