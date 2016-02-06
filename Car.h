#pragma once
class Car
{
public:
	Car();
	~Car();


	//Funktionen

	sf::Vector2f GetPos() { return  _Sprite.getPosition(); }
	

	//Variablen

private:
	sf::Sprite _Sprite;
	sf::Texture _Texture;
	int _Health;



};

