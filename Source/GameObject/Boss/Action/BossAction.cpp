
#include "stdafx.h"
#include "GameObject/Boss/Action/BossAction.h"

BossAction::BossAction(std::function<void()> action) : _Action(action), _Executed(false) {}

void BossAction::execute()  {
    std::cout << "Executing action" << std::endl;
    if (_Action != nullptr) {
        _Action();
    }
    _Executed = true;
}
