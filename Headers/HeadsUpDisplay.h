#pragma once

#include <Screen/GameScreen.h>

class HeadsUpDisplay : GameScreen {
public:
    /*
        Takes care of updating and rendering the HUD with all the vital information that the player needs to play the game.
        Displayed are health energy and score.
    */
    HeadsUpDisplay(Framework &framework);

    ~HeadsUpDisplay() {}

    /*
        Renders the HeadsUpDisply to the specified RenderWindow
        @param window Window to draw to
    */
    void render(sf::RenderWindow &window);

    /*
        Updates all the numbers on the HeadsUpDisplay
        @param score Score the player has earned
        @param health Current health of the player
        @param energy Current energy of the player
        @param level Current level
        @param levelTime Current time within the level
        @param mode Current game mode
    */
    void update(float frameTime);

    void handleEvent(sf::Event &event);

    /*
        @param maxHealth Maximum health the player can have
    */
    void setMaxHealth(int maxHealth) { _MaxHealth = maxHealth; }

    /*
        @param maxEnergy Maximum energy the player can have
    */
    void setMaxEnergy(int maxEnergy) { _MaxEnergy = maxEnergy; }

    /*
        @param totalLevelTime Length of the current level in seconds
    */
    void setTotalLevelTime(float totalLevelTime) { _TotalLevelTime = totalLevelTime; }

private:
    sf::Font _Font;
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

    int _MaxHealth, _MaxEnergy;
    float _TotalLevelTime;

    std::string ConvertScore(int score);
};

