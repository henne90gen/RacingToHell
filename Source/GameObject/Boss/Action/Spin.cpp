#include "stdafx.h"
#include "GameObject/Boss/Action/Spin.h"
#include "GameObject/Boss/BossCar.h"

Spin::Spin(BossCar &boss) : BossAction(boss) {}

void Spin::execute() {
    BossAction::checkParentAction();
    if (BossAction::hasBeenExecuted()) {
        _Boss.setGunMovement(0);
    }
    _Boss.setGunMovement(1);
}
