#include "stdafx.h"

#include "Menu\MenuItem\ComboBox.h"

ComboBox::ComboBox(sf::Vector2f &Position, std::vector<std::string> &Items, MenuResult Action) : MenuItem(MenuItemType::MCombobox, Action),
	_Items(Items), _Position(Position),
	_CurrentItem(0), _Height(50), _BodyWidth(100), _ButtonWidth(20), _TextSize(30),
	_BgColorEnabled(sf::Color(0, 0, 0, 175)), _BgColorHovering(sf::Color(50, 50, 50, 100)), _BgColorDisabled(sf::Color(0, 0, 0, 175)), 
	_FgColorEnabled(sf::Color(255, 255, 255, 255)), _FgColorDisabled(sf::Color(150, 150, 150, 255))
{
	_Body.setPosition(Position + sf::Vector2f(_ButtonWidth, 0));
	_Body.setSize(sf::Vector2f(_BodyWidth, _Height));
	_Body.setOutlineThickness(1);
	_Body.setOutlineColor(sf::Color::Black);

	_ButtonLeft.setPosition(Position);
	_ButtonLeft.setSize(sf::Vector2f(_BodyWidth, _Height));
	_ButtonLeft.setOutlineThickness(1);
	_ButtonLeft.setOutlineColor(sf::Color::Black);

	_ButtonRight.setPosition(Position + sf::Vector2f(_ButtonWidth + _BodyWidth, 0));
	_ButtonRight.setSize(sf::Vector2f(_BodyWidth, _Height));
	_ButtonRight.setOutlineThickness(1);
	_ButtonRight.setOutlineColor(sf::Color::Black);

	if (_ArrowTexture.loadFromFile("Resources/Texture/MainMenu/ComboboxArrow.png"))
	{
		_LeftArrowSprite.setTexture(_ArrowTexture);
		_RightArrowSprite.setTexture(_ArrowTexture);

		_LeftArrowSprite.setRotation(180);

		_LeftArrowSprite.setPosition(Position);
		_RightArrowSprite.setPosition(Position + sf::Vector2f(_BodyWidth + _ButtonWidth, 0));
	}
	
	_Text.setFont(_Font);
	_Text.setColor(sf::Color::White);
	_Text.setCharacterSize(_TextSize);
	_Text.setString(_Items[_CurrentItem]);
	_Text.setPosition(Position + sf::Vector2f(_ButtonWidth + _BodyWidth - 0.5f * _Text.getGlobalBounds().width, 0));
}

ComboBox::~ComboBox()
{
}

void ComboBox::render(sf::RenderWindow & RenderWidnow)
{
	if (_Enabled)
	{
		if (_HoveringLeft)
		{
			_ButtonLeft.setFillColor(_BgColorHovering);
			_ButtonRight.setFillColor(_BgColorEnabled);
		}
		else if (_HoveringRight)
		{
			_ButtonRight.setFillColor(_BgColorHovering);
			_ButtonLeft.setFillColor(_BgColorEnabled);
		}
		else
		{
			_ButtonRight.setFillColor(_BgColorEnabled);
			_ButtonLeft.setFillColor(_BgColorEnabled);
		}

		_Body.setFillColor(_BgColorEnabled);
		_Text.setColor(_FgColorEnabled);
	}
	else
	{
		_Body.setFillColor(_BgColorDisabled);
		_ButtonLeft.setFillColor(_BgColorDisabled);
		_ButtonRight.setFillColor(_BgColorDisabled);

		_Text.setColor(_FgColorDisabled);
	}

	RenderWidnow.draw(_ButtonLeft);
	RenderWidnow.draw(_ButtonRight);
	RenderWidnow.draw(_Body);

	RenderWidnow.draw(_Text);

	RenderWidnow.draw(_LeftArrowSprite);
	RenderWidnow.draw(_RightArrowSprite);
}

MenuResult ComboBox::handleEvent(sf::Event & newEvent, sf::Vector2f mousePos)
{
	if (newEvent.type = sf::Event::MouseMoved)
	{
		_HoveringLeft = pointInRectangle(_ButtonLeft.getGlobalBounds(), mousePos);
		_HoveringRight = pointInRectangle(_ButtonRight.getGlobalBounds(), mousePos);
	
		_Hovering = _HoveringLeft || _HoveringRight;
	} 
	else if (newEvent.type = sf::Event::MouseButtonPressed)
	{
		if (pointInRectangle(_ButtonLeft.getGlobalBounds(), mousePos))
		{
			_CurrentItem = (_CurrentItem - 1 < 0) ? _Items.size() - 1 : _CurrentItem - 1;
		}
		else if (pointInRectangle(_ButtonLeft.getGlobalBounds(), mousePos))
		{
			_CurrentItem = (_CurrentItem + 1 >= _Items.size()) ? 0 : _CurrentItem + 1;
		}
	}

	return _Action;
}

sf::FloatRect & ComboBox::getRect()
{
	return _Body.getGlobalBounds();
}

