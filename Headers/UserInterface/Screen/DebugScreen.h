//
// Created by henne on 22.01.17.
//

#pragma once

#include "GameScreen.h"

class DebugScreen : public GameScreen {
public:
    DebugScreen(Framework &framework);

    virtual void render(sf::RenderWindow &renderWindow);

    virtual void handleEvent(sf::Event &event);

    virtual void update(float frameTime);

private:
    unsigned int _CharacterSize = 14;
    int _XOffset = 0;
    int _YOffset = 50;
    int _HorizontalSpacing = 75;
    int _VerticalSpacing = 15;

    std::vector<std::string> _LabelStrings = {"FPS:", "UPS:", "CurrState:", "LastState:", "Score:", "Bullets:", "Cars:",
                                              "AI health:", "Car freq:", "Bullet freq:", "Can. freq:", "Tool. freq:"};

    std::vector<sf::Text> _Labels;
    std::vector<sf::Text> _Values;
};
