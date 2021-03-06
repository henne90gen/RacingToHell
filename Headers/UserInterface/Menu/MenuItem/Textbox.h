#pragma once

#include "StandardCursor.h"
#include "MenuItem.h"

//#include <Windows.h>

class Textbox : public MenuItem<std::string> {
public:
    /**
     * Textbox element that can be placed in a menu
     * @param pos Position of the Textbox
     * @param size Size of the Textbox
     * @param charSize Size of the chars in the Textbox
     * @param text Text that will be displayed in the Textbox by default
     * @param isFocused Whether the Textbox is focused right after initialization
     */
    Textbox(sf::Vector2f pos, sf::Vector2f size, unsigned int characterSize, std::string text, sf::Font &font,
            bool isFocused, bool isPassword);
    Textbox() {}

    virtual ~Textbox() {}

    void render(sf::RenderWindow &renderWindow);

    bool handleEvent(sf::Event &newEvent, sf::Vector2f mousePos);

    sf::FloatRect getRect();

    /*
        @return std::string Text of the Textbox
    */
    std::string getText() { return _String; }

    void setText(std::string str);

    /*
        Textbox doesn't have a value that could be changed
        @return bool False
    */
    void setValue(std::string string) { _String = string; }

    /*
        Textbox doesn't have a value that could be returned
        @return float 0.0f
    */
    std::string getValue() { return _String; }

    /*
        Button doesn't have a maximum value that could be returned
        @return float 0.0f
    */
    std::string getMaxValue() { return ""; }

private:
    std::string _String;

    sf::RectangleShape _Box, _Cursor;
    sf::Color _FillColor, _FillColorDisabled, _OutlineColor, _OutlineColorFocused, _TextColor;
    sf::Text _Text;
    sf::Clock _CursorClock;

    char _PasswordChar;
    bool _ShowCursor, _isPassword;
    unsigned long _CursorPosition;

    /*
        Sets the cursor to the correct position in the text of the Textbox
    */
    void setCursor();

    /*
        @param str String that is going to be checked
        @return bool True if str is too long for the Textbox
    */
    bool isStringTooLarge(std::string str);

    std::string passwordString(std::string s);

    std::string GetClipboardText();
};
