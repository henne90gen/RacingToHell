
#include <GameObject/Boss/Action/MoveToPosition.h>
#include <GameObject/Boss/Action/RotateGunToPosition.h>
#include "GameObject/Boss/Action/MoveLeftRight.h"
#include "GameObject/Boss/BossCar.h"

MoveLeftRight::MoveLeftRight(BossCar &boss, sf::Vector2f leftPos, sf::Vector2f rightPos, unsigned int numSwitches)
        : BossAction(boss), _LeftTargetPosition(leftPos), _RightTargetPosition(rightPos), _LastTargetPosition(rightPos),
          _NumSwitches(numSwitches) {
    _Boss.addAction(std::make_shared<MoveToPosition>(_Boss, _LastTargetPosition));
}

void MoveLeftRight::execute() {
    BossAction::checkParentAction();
    if (BossAction::hasBeenExecuted()) {
        return;
    }

    if (_NumSwitches == 0) {
        BossAction::finishExecution();
        _Boss.setMovement(sf::Vector2f(0, 0));
        _Boss.addAction(std::make_shared<RotateGunToPosition>(_Boss, sf::Vector2f(0, 1)));
        return;
    }

    if (rh::vectorLength(_Boss.getPosition() - _LastTargetPosition) <= 1.5f) {
        if (_LastTargetPosition == _RightTargetPosition) {
            _LastTargetPosition = _LeftTargetPosition;
        } else if (_LastTargetPosition == _LeftTargetPosition) {
            _LastTargetPosition = _RightTargetPosition;
        }
        _Boss.addAction(std::make_shared<MoveToPosition>(_Boss, _LastTargetPosition));
        _NumSwitches--;
    }
}
