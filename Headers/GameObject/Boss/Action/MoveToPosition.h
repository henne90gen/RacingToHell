#pragma once

#include "stdafx.h"
#include "BossAction.h"

/**
 * Moving the boss to a specific position on the screen.
 * This is not very accurate, we might end up 1 or 2 pixels next to the requested position.
 */
class MoveToPosition : public BossAction {
public:
    MoveToPosition(BossCar &boss, sf::Vector2f targetPosition) : BossAction(boss), _TargetPosition(targetPosition) {}

    void execute();

private:
    sf::Vector2f _TargetPosition;
};
