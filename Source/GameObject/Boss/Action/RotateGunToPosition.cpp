
#include "GameObject/Boss/Action/RotateGunToPosition.h"
#include "GameObject/Boss/BossCar.h"

RotateGunToPosition::RotateGunToPosition(BossCar &boss, sf::Vector2f finalPosition) : BossAction(boss),
                                                                                      _FinalPosition(finalPosition) {}

void RotateGunToPosition::execute() {
    BossAction::checkParentAction();

    _Boss.rotateGunTowards(_FinalPosition);

    if (_Boss.getGunMovement() == 0) {
        BossAction::finishExecution();
    }

    if (BossAction::hasBeenExecuted()) {
        _Boss.setGunMovement(0);
    }
}
