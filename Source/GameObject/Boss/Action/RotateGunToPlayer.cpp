#include "GameObject/Boss/Action/RotateGunToPlayer.h"
#include "GameObject/Boss/BossCar.h"
#include "Framework/GameObjectManager.h"

RotateGunToPlayer::RotateGunToPlayer(BossCar &boss, std::shared_ptr<BossAction> parentAction) : BossAction(boss) {
    _ParentAction = parentAction;
}

void RotateGunToPlayer::execute() {
    BossAction::checkParentAction();
    if (BossAction::hasBeenExecuted()) {
        _Boss.setGunMovement(0);
    } else {
        sf::Vector2f newDirection = _Boss.getGOM().getPlayerCar()->getPosition() - _Boss.getGunBasePosition();
        _Boss.rotateGunTowards(newDirection);
    }
}