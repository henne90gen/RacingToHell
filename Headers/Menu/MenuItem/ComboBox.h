#pragma once

#include "MenuItem.h"

class ComboBox : public MenuItem
{
public:
	ComboBox(sf::Vector2f Position, std::vector<std::string> &Items, MenuResult Action);
	~ComboBox();

	/*
	See MenuItem for details
	*/
	void render(sf::RenderWindow &RenderWidnow);

	/*
	See MenuItem for details
	*/
	MenuResult handleEvent(sf::Event & newEvent, sf::Vector2f mousePos);

	/*
	See MenuItem for details
	*/
	sf::FloatRect & getRect();

	/*
	@return std::string Text of the Button
	*/
	std::string getText() { return _Items[_CurrentItem]; }

	/*
	set currentItem
	@return bool False
	*/
	void setValue(float x);

	/*
	return current Item
	@return float 0.0f
	*/
	float getValue() { return _CurrentItem; }

	/*
	Combo doesn't have a maximum value that could be returned
	@return float 0.0f
	*/
	float getMaxValue() { return 0.0f; }
private:
	std::vector<std::string> _Items;

	sf::RectangleShape _ButtonLeft, _ButtonRight, _Body;
	sf::Vector2f _Position;
	sf::Texture _ArrowTexture;
	sf::Sprite _LeftArrowSprite, _RightArrowSprite;
	sf::Text _Text;
	sf::Color _BgColorEnabled, _BgColorHovering, _BgColorDisabled;
	sf::Color _FgColorEnabled, _FgColorDisabled;

	bool _HoveringLeft, _HoveringRight;
	int _CurrentItem, _Height, _ButtonWidth, _BodyWidth, _TextSize;

	void centerText();
};

