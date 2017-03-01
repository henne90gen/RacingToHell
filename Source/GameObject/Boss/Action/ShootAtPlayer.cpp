
#include "GameObject/Boss/Action/ShootAtPlayer.h"
#include "GameObject/Boss/BossCar.h"
#include "Framework/GameObjectManager.h"

ShootAtPlayer::ShootAtPlayer(BossCar &boss, std::shared_ptr<BossAction> parentAction,
                             std::vector<float> shotFrequencies, std::vector<float> salveAngles)
        : BossAction(boss), _ParentAction(parentAction), _ShotFrequencies(shotFrequencies), _SalveAngles(salveAngles) {}

void ShootAtPlayer::execute() {
    if (_ParentAction && _ParentAction->hasBeenExecuted()) {
        _Executed = true;
    }
    if (_Timer.getElapsedTime().asSeconds() >= _ShotFrequencies[_CurrentFrequencyIndex]) {
        for (unsigned int i = 0; i < _SalveAngles.size(); i++) {
            float angle = _SalveAngles[i];
            sf::Vector2f dir = _Boss.getGOM().getPlayerCar()->getPosition() - _Boss.getPosition();
            dir = rh::rotateVector(dir, angle);
            // FIXME use position of end of gun
            _Boss.shootBullet(_Boss.getPosition(), dir);
        }

        _Timer.restart();
        _CurrentFrequencyIndex++;
        if (_CurrentFrequencyIndex >= _ShotFrequencies.size()) {
            _CurrentFrequencyIndex = 0;
        }
    }
}
