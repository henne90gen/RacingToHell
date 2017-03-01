
#include "stdafx.h"
#include "GameObject/Boss/Action/BossAction.h"

BossAction::BossAction(BossCar &boss, std::function<void()> action) : _Action(action), _Executed(false), _Boss(boss) {}

void BossAction::execute()  {
    std::cout << "Executing action" << std::endl;
    if (_Action != nullptr) {
        _Action();
    }
    _Executed = true;
}
