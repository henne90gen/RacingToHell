#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/StatBox.h"
#include "PlayerStats.h"

StatBox::StatBox(sf::Vector2f pos) {
    _Background.setSize(sf::Vector2f(400, 200));
    _Background.setPosition(pos - sf::Vector2f(_Background.getSize().x / 2, 0));
    _Background.setFillColor(sf::Color(0, 0, 0, 100));
    _Background.setOutlineColor(sf::Color::Black);
    _Background.setOutlineThickness(2);

    _Font.loadFromFile("Resources/Font/arial.ttf");
    _Headline.setFont(_Font);
    _Headline.setPosition(_Background.getPosition() + sf::Vector2f(5, 5));
    _Headline.setCharacterSize(28);
    _Headline.setString("Car Statistics");
    _Headline.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);

    _StatBarTexture.loadFromFile("Resources/Texture/MainMenu/StatBar.png");
}

StatBox::~StatBox() {}

void StatBox::render(sf::RenderWindow &window) {
    //Background
    window.draw(_Background);

    //�berschrift
    window.draw(_Headline);

    //Balken
    std::vector<std::string> StatNames = {"Max. Health:", "Max. Energy:", "Speed:", "Damage:"};
    std::vector<int> MaxStats = PlayerStats::getMaxStats();

    for (unsigned int i = 0; i < _PlayerStats.size() && i < StatNames.size(); i++) {
        sf::Text StatName;
        StatName.setFont(_Font);
        StatName.setPosition(_Background.getPosition() + sf::Vector2f(15, 50 + 35 * i));
        StatName.setCharacterSize(20);
        StatName.setString(StatNames.at(i));

        sf::Sprite StatBar;
        StatBar.setTexture(_StatBarTexture);
        StatBar.setPosition(_Background.getPosition() + sf::Vector2f(140, 53 + 35 * i));
        StatBar.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(
                ((int) (float) _PlayerStats[i] / (float) MaxStats[i] * (float) StatBar.getLocalBounds().width), 20)));

        window.draw(StatName);
        window.draw(StatBar);
    }
}

void StatBox::setCarStats(PlayerCarIndex index) {
    _PlayerStats = PlayerStats::getPlayerStats(index);
}
