
#include <GameObject/Boss/Action/PointGunAtPlayer.h>
#include "GameObject/Boss/Action/ShootAtPlayer.h"
#include "GameObject/Boss/BossCar.h"

ShootAtPlayer::ShootAtPlayer(BossCar &boss, std::vector<float> shotFrequencies, std::vector<float> salveAngles)
        : BossAction(boss), _ShotFrequencies(shotFrequencies), _SalveAngles(salveAngles), _PointingAtPlayer(false) {}

void ShootAtPlayer::execute() {
    if (!_PointingAtPlayer) {
        _Boss.addAction(std::make_shared<PointGunAtPlayer>(_Boss, shared_from_this()));
        _PointingAtPlayer = true;
    }
    if (_ParentAction && _ParentAction->hasBeenExecuted()) {
        BossAction::finishExecution();
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
