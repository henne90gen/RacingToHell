#include "stdafx.h"
#include "UserInterface/Menu/MenuItem/PlayerTable.h"

PlayerTable::PlayerTable(sf::Vector2f Position, sf::Font &font) : MenuItem(MenuItemType::MLobbyList,
                                                                           MenuResult::Nothing, font),
                                                                  _Position(Position), _Size(550, 80) {
    _Background.setFillColor(sf::Color(0, 0, 0, 0));
    _Background.setOutlineThickness(1);
    _Background.setOutlineColor(sf::Color::Black);
    _Background.setPosition(_Position);
    _Background.setSize(_Size);

    _AdminIcon.loadFromFile("Resources/Texture/PlayerTable/Admin.png");
    _ReadyIcon.loadFromFile("Resources/Texture/PlayerTable/Tick.png");
    _CrossIcon.loadFromFile("Resources/Texture/PlayerTable/Cross.png");
    _TrashIcon.loadFromFile("Resources/Texture/PlayerTable/Trash.png");

    _AdminIcon.setSmooth(true);
    _ReadyIcon.setSmooth(true);
    _CrossIcon.setSmooth(true);
}

void PlayerTable::render(sf::RenderWindow &window) {
    window.draw(_Background);

    for (unsigned int i = 0; i < 2; i++) {
        sf::RectangleShape Background;
        sf::Text PlayerName, ScoreText;
        sf::Sprite LeftSprite, RightSprite;

        Background.setOutlineColor(sf::Color::Black);
        Background.setOutlineThickness(1);
        Background.setPosition(_Position + sf::Vector2f(0, 40 * i));
        Background.setSize(sf::Vector2f(_Size.x, 40));

        if (i < _MemberList.size()) {
            if (_MemberList[i]._Ready) {
                Background.setFillColor(sf::Color(110, 190, 110, 100));
                LeftSprite.setTexture(_ReadyIcon);
            } else {
                Background.setFillColor(sf::Color(190, 110, 110, 100));
                LeftSprite.setTexture(_CrossIcon);
            }

            if (_MemberList[i]._Admin) {
                LeftSprite.setTexture(_AdminIcon);
            } else {
                RightSprite.setTexture(_TrashIcon);
                RightSprite.setPosition(_Position + sf::Vector2f(_Size.x - 35, 5 + 40 * i));
            }

            LeftSprite.setPosition(_Position + sf::Vector2f(5, 5 + 40 * i));

            PlayerName.setFont(_Font);
            PlayerName.setCharacterSize(27);
            PlayerName.setString(_MemberList[i]._Name);
            PlayerName.setPosition(_Position + sf::Vector2f(50, 40 * i));

            ScoreText = PlayerName;
            ScoreText.setString(std::to_string(_MemberList[i]._Score));
            ScoreText.setPosition(_Position + sf::Vector2f(_Size.x - 50 - ScoreText.getGlobalBounds().width, 40 * i));
        } else {
            Background.setFillColor(sf::Color(0, 0, 0, 120));
        }

        window.draw(Background);
        window.draw(PlayerName);
        window.draw(ScoreText);
        window.draw(LeftSprite);

        if (_isAdmin) {
            window.draw(RightSprite);
        }
    }
}

MenuResult PlayerTable::handleEvent(sf::Event &newEvent, sf::Vector2f mousePos) {
    if (_isAdmin) {
        for (unsigned int i = 0; i < 2; i++) {
            if (i < _MemberList.size() && !_MemberList[i]._Admin) {
                sf::FloatRect Rect(_Position + sf::Vector2f(_Size.x - 35, 40 * i), sf::Vector2f(30, 40));

                if (rh::pointInRectangle(Rect, mousePos)) {
                    _Hovering = true;

                    if (newEvent.type == sf::Event::MouseButtonPressed) {
                        return MenuResult::KickOtherPlayer;
                    }

                    break;
                } else {
                    _Hovering = false;
                }
            }
        }

    }

    if (_MemberList.size() == 1) {
        _Hovering = false;
    }

    return MenuResult::Nothing;
}

void PlayerTable::addPlayer(std::string name, bool isAdmin) {
    LobbyMember newMember;
    newMember._Name = name;

    if (isAdmin) {
        newMember._Ready = true;
        newMember._Admin = true;
    }

    newMember._Score = 0;

    _MemberList.push_back(newMember);
}

void PlayerTable::removePlayer(int index) {
    if (index >= 0 && index < _MemberList.size()) {
        _MemberList.erase(_MemberList.begin() + index);
    }
}

void PlayerTable::resetTable() {
    while (_MemberList.size() > 0) {
        removePlayer(0);
    }
}

void PlayerTable::setMember(unsigned int index, bool ready, int score, std::string name) {
    if (index < _MemberList.size()) {
        _MemberList[index]._Ready = ready;

        if (name != "") {
            _MemberList[index]._Name = name;
        }

        if (score >= 0) {
            _MemberList[index]._Score = score;
        }
    }
}

bool PlayerTable::getPlayerReady() {
    return _MemberList.size() == 2 && _MemberList[1]._Ready;
}
