
#include "stdafx.h"
#include "GameObject/Boss/Action/BossAction.h"
#include "GameObject/Boss/BossCar.h"

BossAction::BossAction(BossCar &boss, std::function<void()> action) : _Action(action), _Executed(false), _Boss(boss) {}

void BossAction::execute() {
    if (_Action != nullptr) {
        _Action();
    }
    finishExecution();
}

void BossAction::finishExecution() {
    _Executed = true;
    if (_NextAction) {
        _Boss.addAction(_NextAction);
    }
}

void BossAction::checkParentAction() {
    if (_ParentAction && _ParentAction->hasBeenExecuted()) {
        BossAction::finishExecution();
    }
}
