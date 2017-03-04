#include "GameObject/Boss/Action/PointGunAtPlayer.h"
#include "GameObject/Boss/BossCar.h"
#include "Framework/GameObjectManager.h"

PointGunAtPlayer::PointGunAtPlayer(BossCar &boss, std::shared_ptr<BossAction> parentAction) : BossAction(boss) {
    _ParentAction = parentAction;
}

void PointGunAtPlayer::execute() {
    if (_ParentAction && _ParentAction->hasBeenExecuted()) {
        BossAction::finishExecution();
    }
    sf::Vector2f dir = _Boss.getGOM().getPlayerCar()->getPosition() - _Boss.getGunBasePosition();
    _Boss.moveGunTowards(dir);
}
