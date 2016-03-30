#pragma once

#include "Menu\MenuItem\MenuItem.h"

class PlayerTable : public MenuItem
{
public:
	PlayerTable(sf::Vector2f Position);
	~PlayerTable() {}

	/*
	Renders the MenuItem to the specified RenderWindow
	@param window Window to draw to
	*/
	void render(sf::RenderWindow & window);

	/*
	Handles events for the MenuItem
	@param newEvent Event to be handled
	*/
	MenuResult handleEvent(sf::Event & newEvent, sf::Vector2f mousePos);

	/*
	See MenuItem for details
	*/
	sf::FloatRect getRect() { return _Background.getGlobalBounds(); };

	/*
	@return std::string Text of the Button
	*/
	std::string getText() { return ""; }

	/*
	Button doesn't have a value that could be changed
	@return bool False
	*/
	void setValue(float x) {}

	/*
	Button doesn't have a value that could be returned
	@return float 0.0f
	*/
	float getValue() { return 0.0f; }

	/*
	Button doesn't have a maximum value that could be returned
	@return float 0.0f
	*/
	float getMaxValue() { return 0.0f; }
private:
	struct LobbyMembers
	{
		std::string _Name = "";
		bool _Ready = false;
		bool _Admin = false;
		int _Score = 0;
	};

	std::vector<LobbyMembers> _MemberList;

	sf::Vector2f _Position, _Size;

	sf::RectangleShape _Background;
	sf::Texture _AdminIcon, _ReadyIcon, _CrossIcon, _TrashIcon;

	int _MyIndex;

	void addPlayer(std::string name, bool isAdmin);
	void removePlayer(int index);
};
