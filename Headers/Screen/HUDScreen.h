#pragma once

#include <Screen/GameScreen.h>

class HUDScreen : public GameScreen {
public:
    /*
        Takes care of updating and rendering the HUD with all the vital information that the player needs to play the game.
        Displayed are health energy and score.
    */
    HUDScreen(Framework &framework);

    ~HUDScreen() {}

    /*
        Renders the HeadsUpDisply to the specified RenderWindow
        @param window Window to draw to
    */
    void render(sf::RenderWindow &window);

    /*
        Updates all the numbers that are being displayed on the HUDScreen
    */
    void update(float frameTime);

    void handleEvent(sf::Event &event);

private:
    sf::Texture _BackgroundTexture;
    sf::Sprite _Background;

    sf::Text _LevelText;
    sf::RectangleShape _ProgressBar;
    sf::RectangleShape _ProgressBarOutline;

    sf::RectangleShape _HealthBar;
    sf::RectangleShape _HealthBarOutline;
    sf::Text _HealthText;
    sf::Texture _HealthIconTexture;
    sf::Sprite _HealthIconSprite;

    sf::RectangleShape _EnergyBar;
    sf::RectangleShape _EnergyBarOutline;
    sf::Text _EnergyText;
    sf::Texture _EnergyIconTexture;
    sf::Sprite _EnergyIconSprite;

    sf::Text _ScoreText;

    std::string ConvertScore(int score);
};

