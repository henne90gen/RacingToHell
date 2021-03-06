#pragma once

#include "Menu.h"
#include "Multiplayer/Communication.h"
#include "Multiplayer/NetworkHandle.h"

#include "GameObject/GameObjectFactory.h"

#include <thread>
#include <future>

class MultiplayerMenu : Menu {
public:
    MultiplayerMenu(Framework &framework);

    virtual ~MultiplayerMenu() {}

    /*
    Renders the OptionsMenu to the specified RenderWindow
    @param window Window to draw to
    */
    void render(sf::RenderWindow &Window);

    /*
    Handles events for the OptionsMenu
    @param window RenderWindow to poll the events from
    @return GameState New GameState the game should be in
    */
    void handleEvent(sf::Event &event);

    void update(float frametime);
//    NetworkCommunication update(float frametime);

    bool getCreatedLobby() { return (bool) _CreatedLobby; }

    void setNetworkHandle(NetworkHandle *Handle) { _NetworkHandle = Handle; }

    std::string getPlayerName();

    void setPlayerName(std::string name);

    void resetFeedback();

    void resetTextbox();

    void setKickMessage();

    void setLobbyClosedMessage();

private:
    sf::RectangleShape _BackgroundJoin, _BackgroundName, _BackgroundCreate;
    sf::Text _NameText, _JoinText, _IPText, _PasswordText, _FeedbackText, _CreateText, _CreatePassword, _FeedbackTextCreate;

    int _CreatedLobby;
    NetworkHandle *_NetworkHandle;

    enum class MenuItemIndex {
        Name = 0, IP = 1, Port = 2, PasswordJoin = 3, PasswordCreate = 5, PortCreate = 6
    };

    // std::thread _ConnectionThread;
    // std::mutex _Mutex;
};
