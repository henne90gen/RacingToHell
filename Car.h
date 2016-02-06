#pragma once

#include "GameObject.h"

class Car : public GameObject
{
public:
	Car();
	~Car();


	//Funktionen

	sf::Vector2f GetPos() { return  _Sprite.getPosition(); }
	void render(sf::RenderWindow& Window);
	void handleEvent(sf::Event& Event);
	void update(float FrameTime);

	//Variablen

private:
	sf::Sprite _Sprite;
	sf::Texture _Texture;
	int _Health;



};

