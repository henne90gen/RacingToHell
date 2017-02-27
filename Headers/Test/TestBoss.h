#pragma once

#include <GameObject/Boss/BossCar.h>

class TestBoss : public BossCar {
public:
    TestBoss(sf::Texture &texture) : BossCar(0, sf::Vector2f(), 0, 0, 0, texture, texture) { init(); }

    void render(sf::RenderWindow &renderWindow) {}

    void update(float frameTime, int roadSpeed, std::vector<std::shared_ptr<Bullet>> &bullets, PlayerCar &player) {}

    void update(float frameTime) {
        GameObject::update(frameTime);
    }

    void init() {}

    void shootBullet(std::vector<std::shared_ptr<Bullet>> &bullets, sf::Vector2f pos, sf::Vector2f dir, int bulletSpeed) {}

};
