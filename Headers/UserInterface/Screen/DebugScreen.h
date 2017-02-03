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
    int _HorizontalSpacing = 5;
    int _VerticalSpacing = 15;

    std::vector<std::string> _LabelStrings;
    std::vector<sf::Text> _Labels;
    std::vector<sf::Text> _Values;
};
