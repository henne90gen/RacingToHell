#pragma once

class StatBox {
public:
    /*
        Statistics for PlayerCars are displayed in here
        @param pos Position of the StatBox
    */
    StatBox(sf::Vector2f pos);

    ~StatBox();

    /*
        Renders the StatBox to the specified RenderWindow
        @param window Window to draw to
        @param selectedCar Index of the car the player has selected
    */
    void render(sf::RenderWindow &window);

    void setCarStats(PlayerCarIndex index);

private:
    sf::RectangleShape _Background;
    sf::Font _Font;
    sf::Text _Headline;
    sf::Texture _StatBarTexture;

    std::vector<int> _PlayerStats;
};