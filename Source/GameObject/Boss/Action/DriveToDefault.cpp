//
// Created by Henne on 2/26/2017.
//

#include "GameObject/Boss/Action/DriveToDefault.h"

void DriveToDefault::execute() {
    int speed = _Boss.getSpeed();
    sf::Vector2f dir = _Boss.getDefaultPosition() - _Boss.getPosition();
    if (rh::vectorLength(dir) <= 1) {
        _Executed = true;
        _Boss.setMovement(sf::Vector2f(0, 0));
        return;
    }
    dir = rh::normalize(dir) * (float) speed;
    _Boss.setMovement(dir);
}
