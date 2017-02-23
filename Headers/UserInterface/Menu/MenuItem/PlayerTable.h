#pragma once

#include "MenuItem.h"

// FIXME maybe use std::vector
class PlayerTable : public MenuItem<float> {
public:
    PlayerTable(sf::Vector2f Position, sf::Font &font);

    ~PlayerTable() {}

    void render(sf::RenderWindow &window);

    bool handleEvent(sf::Event &newEvent, sf::Vector2f mousePos);

    sf::FloatRect getRect() { return _Background.getGlobalBounds(); };

    /**
     * @return std::string Text of the Button
     */
    std::string getText() { return ""; }

    /**
     * Button doesn't have a value that could be changed
     * @return bool False
     */
    void setValue(float x) {}

    /**
     * Button doesn't have a value that could be returned
     * @return float 0.0f
     */
    float getValue() { return 0.0f; }

    /**
     * Button doesn't have a maximum value that could be returned
     * @return float 0.0f
     */
    float getMaxValue() { return 0.0f; }

    void addPlayer(std::string name, bool isAdmin);

    void removePlayer(int index);

    void resetTable();

    void setAdmin(bool admin) { _isAdmin = admin; };

    void setMember(unsigned int index, bool ready, int score = -1, std::string name = "");

    bool getPlayerReady();

private:
    struct LobbyMember {
        std::string _Name = "";
        bool _Ready = false;
        bool _Admin = false;
        int _Score = 0;
    };

    std::vector<LobbyMember> _MemberList;

    sf::Vector2f _Position, _Size;

    sf::RectangleShape _Background;
    sf::Texture _AdminIcon, _ReadyIcon, _CrossIcon, _TrashIcon;

    bool _isAdmin;
};
