#include "stdafx.h"

#include "Menu\MenuItem\ComboBox.h"

ComboBox::ComboBox(sf::Vector2f Position, std::vector<std::string> &Items, MenuResult Action) : MenuItem(MenuItemType::MCombobox, Action),
	_Items(Items), _Position(Position),
	_CurrentItem(0), _Height(50), _BodyWidth(160), _ButtonWidth(20), _TextSize(40),
	_BgColorEnabled(sf::Color(0, 0, 0, 175)), _BgColorHovering(sf::Color(50, 50, 50, 100)), _BgColorDisabled(sf::Color(0, 0, 0, 175)), 
	_FgColorEnabled(sf::Color(255, 255, 255, 255)), _FgColorDisabled(sf::Color(150, 150, 150, 255))
{
	_Body.setPosition(Position + sf::Vector2f(_ButtonWidth, 0));
	_Body.setSize(sf::Vector2f(_BodyWidth, _Height));
	_Body.setOutlineThickness(1);
	_Body.setOutlineColor(sf::Color::Black);

	_ButtonLeft.setPosition(Position);
	_ButtonLeft.setSize(sf::Vector2f(_ButtonWidth, _Height));
	_ButtonLeft.setOutlineThickness(1);
	_ButtonLeft.setOutlineColor(sf::Color::Black);

	_ButtonRight.setPosition(Position + sf::Vector2f(_ButtonWidth + _BodyWidth, 0));
	_ButtonRight.setSize(sf::Vector2f(_ButtonWidth, _Height));
	_ButtonRight.setOutlineThickness(1);
	_ButtonRight.setOutlineColor(sf::Color::Black);

	if (_ArrowTexture.loadFromFile("Resources/Texture/MainMenu/ComboboxArrow.png"))
	{
		_LeftArrowSprite.setTexture(_ArrowTexture);
		_RightArrowSprite.setTexture(_ArrowTexture);

		_LeftArrowSprite.setTextureRect(sf::IntRect(_ArrowTexture.getSize().x, 0, -1 * _ArrowTexture.getSize().x, _ArrowTexture.getSize().y));

		_LeftArrowSprite.setPosition(Position + sf::Vector2f(0.5f * _ButtonWidth - 0.5f * _ArrowTexture.getSize().x, 0.5f * _Height - 0.5f * _ArrowTexture.getSize().y));
		_RightArrowSprite.setPosition(Position + sf::Vector2f(_BodyWidth + _ButtonWidth + 0.5f * _ButtonWidth - 0.5f * _ArrowTexture.getSize().x, 0.5f * _Height - 0.5f * _ArrowTexture.getSize().y));
	}
	
	_Text.setFont(_Font);
	_Text.setColor(sf::Color::White);
	_Text.setCharacterSize(_TextSize);
	_Text.setString(_Items[_CurrentItem]);
	centerText();
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
	if (_Enabled)
	{
		if (newEvent.type == sf::Event::MouseMoved)
		{
			_HoveringLeft = pointInRectangle(_ButtonLeft.getGlobalBounds(), mousePos);
			_HoveringRight = pointInRectangle(_ButtonRight.getGlobalBounds(), mousePos);

			_Hovering = _HoveringLeft || _HoveringRight;
		}
		if (newEvent.type == sf::Event::MouseButtonPressed)
		{
			if (pointInRectangle(_ButtonLeft.getGlobalBounds(), mousePos))
			{
				_CurrentItem = (_CurrentItem - 1 < 0) ? _Items.size() - 1 : _CurrentItem - 1;
			}
			else if (pointInRectangle(_ButtonRight.getGlobalBounds(), mousePos))
			{
				_CurrentItem = (_CurrentItem + 1 >= _Items.size()) ? 0 : _CurrentItem + 1;	
			}

			_Text.setString(_Items[_CurrentItem]);
			centerText();
		} 

	}

	return MenuResult::Nothing;
}

sf::FloatRect ComboBox::getRect()
{
	return _Body.getGlobalBounds();
}

void ComboBox::centerText()
{
	_Text.setPosition(_Position + sf::Vector2f(_ButtonWidth + 0.5f * _BodyWidth - 0.5f * (float)_Text.getGlobalBounds().width, 0));
}

void ComboBox::setValue(float x)
{
	_CurrentItem = ((int)x % _Items.size());
	_Text.setString(_Items[_CurrentItem]);
	centerText();
}
