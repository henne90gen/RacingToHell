#pragma once

#include "MenuItemType.h"
#include "UserInterface/Menu/AlignText.h"

template<class T>
class MenuItem {
public:

    /**
     * Interface for all the things that can be part of a menu
     * @param type Type of MenuItem
     * @param action Action the Button is associated with
     */
    MenuItem(MenuItemType type, sf::Font &font) : _Enabled(true), _Type(type), _Focused(false) {}

    MenuItem() {}

    virtual ~MenuItem() {}

    /**
     * Renders the MenuItem to the specified RenderWindow
     * @param window Window to draw to
     */
    virtual void render(sf::RenderWindow &renderWindow) = 0;

    /**
     * Handles events for the MenuItem
     * @param newEvent: Event to be handled
     */
    virtual bool handleEvent(sf::Event &newEvent, sf::Vector2f mousePos) = 0;

    /**
     * @return sf::FloatRect: Bounding box of the MenuItem
     */
    virtual sf::FloatRect getRect() = 0;

    /**
     * @return std::string Text of the MenuItem
     */
    virtual std::string getText() = 0;

    virtual void setText(std::string str) {}

    /**
     * @param value New value for the MenuItem
     */
    virtual void setValue(T value) = 0;

    /**
     * @return float Value of the MenuItem
     */
    virtual T getValue() = 0;

    /**
     * @return float Maximum value for the MenuItem
     */
    virtual T getMaxValue() = 0;

    /**
     * @return bool True if the mouse is hovering over the MenuItem
     */
    bool getHoverState() { return _Hovering; }

    /**
     * @param hover New hover state
     */
    void setHoverState(bool hover) { _Hovering = hover; }

    /**
     * @return bool True if the MenuItem is enabled
     */
    bool getEnabled() { return _Enabled; }

    /**
     * @param enabled True for enabled
     */
    void setEnabled(bool enabled) { _Enabled = enabled; }

    /**
     * @return bool True if MenuItem is focused
     */
    bool getFocused() { return _Focused; }

    /**
     * @param focused New focus state
     */
    void setFocused(bool focused) { _Focused = focused; }

    /**
     * @param visible
     */
    void setVisible(bool visible) { _Visible = visible; }

// TODO check whether we need these methods
//    virtual void resetTable() {}
//    virtual void addPlayer(std::string name, bool admin) {}
//    virtual void removePlayer(int index) {}
//    virtual void setAdmin(bool admin) {}
//    virtual void setMember(unsigned int index, bool ready, int score = -1, std::string name = "") {}
//    virtual bool getPlayerReady() { return false; }

    MenuItemType getType() { return _Type; }

protected:
    MenuItemType _Type;
    bool _Hovering, _Enabled, _Focused, _Visible;
};

