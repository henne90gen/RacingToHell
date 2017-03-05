
#include <GameObject/Boss/Action/RotateGunToPlayer.h>
#include "GameObject/Boss/Action/Shoot.h"
#include "GameObject/Boss/BossCar.h"

Shoot::Shoot(BossCar &boss, std::vector<float> shotFrequencies, std::vector<float> salveAngles)
        : BossAction(boss), _ShotFrequencies(shotFrequencies), _SalveAngles(salveAngles), _PointingAtPlayer(false) {}

void Shoot::execute() {
    BossAction::checkParentAction();
    if (BossAction::hasBeenExecuted()) {
        return;
    }

    if (_Timer.getElapsedTime().asSeconds() >= _ShotFrequencies[_CurrentFrequencyIndex]) {
        for (unsigned int i = 0; i < _SalveAngles.size(); i++) {
            float angle = _SalveAngles[i];
            sf::Vector2f dir = rh::rotateVector(_Boss.getGunDirection(), angle);
            _Boss.shootBullet(_Boss.getGunEnd(), dir);
        }

        _Timer.restart();
        _CurrentFrequencyIndex++;
        if (_CurrentFrequencyIndex >= _ShotFrequencies.size()) {
            _CurrentFrequencyIndex = 0;
        }
    }
}
