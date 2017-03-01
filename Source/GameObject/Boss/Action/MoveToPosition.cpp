#include "stdafx.h"
#include "GameObject/Boss/BossCar.h"
#include "GameObject/Boss/Action/MoveToPosition.h"

void MoveToPosition::execute() {
    int speed = _Boss.getSpeed();
    sf::Vector2f dir = _TargetPosition - _Boss.getPosition();
    // Being too accurate causes the boss to just shake in place
    if (rh::vectorLength(dir) <= 1.5f) {
        _Executed = true;
        // Reset movement to stop boss from moving over the target
        _Boss.setMovement(sf::Vector2f(0, 0));
        return;
    }
    dir = rh::normalize(dir) * (float) speed;
    _Boss.setMovement(dir);
}
