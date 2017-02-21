#pragma once

#include "GameScreen.h"

class Framework;

class LevelUpScreen : public GameScreen {
public:

    LevelUpScreen(Framework &framework);

    ~LevelUpScreen() {}

    /**
     * Renders the HighscoreMenu to the specified RenderWindow
     * @param window Window to draw to
     */
    virtual void render(sf::RenderWindow &window);

    virtual void handleEvent(sf::Event &event) {}

    /**
     * Updates the LevelUpScreen and all the animations
     * @return bool True when the animations are done
     */
    virtual void update(float frameTime);

private:
    sf::Text _Text;

    sf::Texture _FasterTexture;
    sf::Sprite _Faster;
    sf::Texture _StrongerTexture;
    sf::Sprite _Stronger;

    sf::RectangleShape _Background;

    bool _SoundPlayed;
    float _SoundDelay;

    sf::Clock _Timer;
    sf::Clock _FadeTimer;

    float _ShowTime, _FadeTime;
};

