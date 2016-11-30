//
// Created by henne on 29.11.16.
//

#pragma once


#include <Screen/GameScreen.h>

class LevelScreen : public GameScreen {
public:
    LevelScreen(Framework &framework);

    virtual void render(sf::RenderWindow &renderWindow);

    virtual void handleEvent(sf::Event &event);

    virtual void update(float frameTime);

private:
    std::vector<std::shared_ptr<sf::Texture>> _Textures;
    sf::Sprite _Sprite;
    int _CurrentLevel;
};


