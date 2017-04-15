
#include "GameObject/Boss/Action/TimerAction.h"

void TimerAction::execute() {
    if (_Timer.getElapsedTime().asSeconds() >= _Time) {
        BossAction::finishExecution();
    }
}
