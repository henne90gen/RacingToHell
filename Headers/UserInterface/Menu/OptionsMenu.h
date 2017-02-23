#pragma once

#include "Menu.h"
#include "MultiplayerMenu.h"
#include "UserInterface/Menu/MenuItem/CheckBox.h"

class OptionsMenu : public Menu {
public:

    OptionsMenu(Framework &framework);

    virtual ~OptionsMenu() {}

    /**
     * Renders the OptionsMenu to the specified RenderWindow
     * @param window Window to draw to
     */
    virtual void render(sf::RenderWindow &renderWindow);

    /**
     * Handles events for the OptionsMenu
     * @param window RenderWindow to poll the events from
     * @return GameState New GameState the game should be in
     */
    virtual void handleEvent(sf::Event &event);

    void update(float FrameTime);

private:
    int _ChangeSliderValue;

    sf::Text _FPS, _Volume, _ScoreMultiplierText;
    sf::RectangleShape _FPSBackground, _VolumeBackground, _ScoreMultiplierBackground;

    Slider _VolumeSlider, _FpsSlider;
    MenuButton _CreditsBtn, _BackBtn;
    CheckBox _VSyncCheck, _DebugCheck;
    ComboBox<Difficulty> _DifficultyCombo;
    ComboBox<GameMode> _GameModeCombo;
};

