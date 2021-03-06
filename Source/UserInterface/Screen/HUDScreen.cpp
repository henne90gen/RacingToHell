﻿#include "stdafx.h"
#include "UserInterface/Screen/HUDScreen.h"
#include "Framework/Framework.h"

HUDScreen::HUDScreen(Framework &framework) : GameScreen(framework) {

    //Displays the current level
    _LevelText.setFont(_FW.getOptionsManager().getFont());
    _LevelText.setPosition(sf::Vector2f(0, 10));

    //Progressbar
    _ProgressBar.setFillColor(sf::Color(255, 255, 255, 100));
    _ProgressBar.setSize(sf::Vector2f(SCREENWIDTH, 10));
    _ProgressBar.setPosition(0, 0);

    //Progressbar outline
    _ProgressBarOutline.setFillColor(sf::Color::Transparent);
    _ProgressBarOutline.setOutlineColor(sf::Color(20, 0, 0));
    _ProgressBarOutline.setOutlineThickness(1);
    _ProgressBarOutline.setSize(_ProgressBar.getSize());

    //Background
    _BackgroundTexture.loadFromFile("Resources/Texture/HUD/HUD_Background.png");
    _Background.setTexture(_BackgroundTexture);
    _Background.setPosition(sf::Vector2f(0, SCREENHEIGHT - _Background.getLocalBounds().height));

    //Healthbar
    _HealthBar.setPosition(sf::Vector2f(_Background.getGlobalBounds().left + 10,
                                        SCREENHEIGHT - _Background.getLocalBounds().height + 5));
    _HealthBar.setSize(sf::Vector2f(150, 30));
    _HealthBar.setFillColor(sf::Color(175, 0, 0));

    //Healthbar Outline
    _HealthBarOutline.setPosition(_HealthBar.getPosition());
    _HealthBarOutline.setSize(_HealthBar.getSize());
    _HealthBarOutline.setFillColor(sf::Color::Transparent);
    _HealthBarOutline.setOutlineColor(sf::Color::Black);
    _HealthBarOutline.setOutlineThickness(2);

    //Health Text
    _HealthText.setFont(_FW.getOptionsManager().getFont());
    _HealthText.setCharacterSize(20);

    //Health Icon
    _HealthIconTexture.loadFromFile("Resources/Texture/HUD/HUD_HealthIcon.png");
    _HealthIconSprite.setTexture(_HealthIconTexture);
    _HealthIconSprite.setPosition(
            sf::Vector2f(_HealthBarOutline.getGlobalBounds().left + 6, _HealthBarOutline.getGlobalBounds().top + 6));

    //Energybar
    _EnergyBar.setPosition(
            sf::Vector2f(_HealthBarOutline.getGlobalBounds().left + _HealthBarOutline.getLocalBounds().width + 20,
                         _HealthBarOutline.getGlobalBounds().top + 2));
    _EnergyBar.setSize(sf::Vector2f(150, 30));
    _EnergyBar.setFillColor(sf::Color(125, 0, 175));

    //Energybar Outline
    _EnergyBarOutline.setPosition(_EnergyBar.getPosition());
    _EnergyBarOutline.setSize(_EnergyBar.getSize());
    _EnergyBarOutline.setFillColor(sf::Color::Transparent);
    _EnergyBarOutline.setOutlineColor(sf::Color::Black);
    _EnergyBarOutline.setOutlineThickness(2);

    //Enrgy Text
    _EnergyText.setFont(_FW.getOptionsManager().getFont());
    _EnergyText.setCharacterSize(20);

    //Energy Icon
    _EnergyIconTexture.loadFromFile("Resources/Texture/HUD/HUD_EnergyIcon.png");
    _EnergyIconSprite.setTexture(_EnergyIconTexture);
    _EnergyIconSprite.setPosition(
            sf::Vector2f(_EnergyBarOutline.getGlobalBounds().left + 6, _EnergyBarOutline.getGlobalBounds().top + 6));

    //Score
    _ScoreText.setFont(_FW.getOptionsManager().getFont());
    _ScoreText.setCharacterSize(30);
    _ScoreText.setPosition(sf::Vector2f(350, SCREENHEIGHT - _Background.getLocalBounds().height));
    _ScoreText.setString("Score: 00000000");
}

void HUDScreen::render(sf::RenderWindow &window) {
    window.draw(_LevelText);

    window.draw(_ProgressBar);
    window.draw(_ProgressBarOutline);

    window.draw(_Background);

    window.draw(_HealthBar);
    window.draw(_HealthBarOutline);
    window.draw(_HealthText);
    window.draw(_HealthIconSprite);

    window.draw(_EnergyBar);
    window.draw(_EnergyBarOutline);
    window.draw(_EnergyText);
    window.draw(_EnergyIconSprite);

    window.draw(_ScoreText);
}

void HUDScreen::update(float frameTime) {

    _LevelText.setString("Level: " + std::to_string(_FW.getLevelManager().getLevel()));

    _ProgressBar.setSize(
            sf::Vector2f(_FW.getLevelManager().getLevelTime() * SCREENWIDTH / _FW.getLevelManager().getTotalLevelTime(),
                         10));

    int health = _FW.getGOM().getPlayerCar()->getHealth();
    int maxHealth = _FW.getGOM().getPlayerCar()->getMaxHealth();

    _HealthBar.setSize(sf::Vector2f((float) health / (float) maxHealth * 150, 30));
    _HealthText.setPosition(sf::Vector2f(
            _HealthBarOutline.getGlobalBounds().left + _HealthBarOutline.getLocalBounds().width -
            _HealthText.getLocalBounds().width - 7, _HealthBarOutline.getGlobalBounds().top + 3));

    // TODO reenable invincibility mode
//    if (mode == GameMode::Invincible) {
//        _HealthText.setString(L"∞");
//        _HealthText.setCharacterSize(40);
//        _HealthText.setPosition(_HealthText.getPosition() - sf::Vector2f(5, 12));
//    } else {
    _HealthText.setString(std::to_string(health) + "/" + std::to_string(maxHealth));
    _HealthText.setCharacterSize(20);
//    }

    if (health <= 10) {
        _HealthText.setFillColor(sf::Color(255, 75, 75));
    } else {
        _HealthText.setFillColor(sf::Color::White);
    }

    int energy = (int) _FW.getGOM().getPlayerCar()->getEnergy();
    int maxEnergy = _FW.getGOM().getPlayerCar()->getMaxEnergy();

    _EnergyBar.setSize(sf::Vector2f((float) energy / (float) maxEnergy * 150, 30));
    _EnergyText.setPosition(sf::Vector2f(
            _EnergyBarOutline.getGlobalBounds().left + _EnergyBarOutline.getLocalBounds().width -
            _EnergyText.getLocalBounds().width - 7, _EnergyBarOutline.getGlobalBounds().top + 3));

    // TODO reenable infinite energy mode
//    if (mode == GameMode::InfEnergy) {
//        _EnergyText.setString(L"∞");
//        _EnergyText.setCharacterSize(40);
//        _EnergyText.setPosition(sf::Vector2f(_EnergyText.getPosition() - sf::Vector2f(5, 12)));
//    } else {
    _EnergyText.setString(std::to_string(energy) + "/" + std::to_string(maxEnergy));
//    }

    if (energy <= 10) {
        _EnergyText.setFillColor(sf::Color(255, 75, 75));
    } else {
        _EnergyText.setFillColor(sf::Color::White);
    }

    _ScoreText.setString("Score: " + formatScore(_FW.getLevelManager().getScore()));
}

std::string HUDScreen::formatScore(int score) {
    std::string scoreString;
    int ScoreLength = (int) std::to_string(score).length();

    if (ScoreLength < 7) {
        for (unsigned int i = 0; i < 8 - ScoreLength; i++) {
            scoreString += "0";
        }

        scoreString += std::to_string(score);
    } else {
        scoreString = std::to_string(score);
    }

    return scoreString;
}
