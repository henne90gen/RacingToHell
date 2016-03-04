#include "stdafx.h"
#include "GameObject\PlayerCar.h"


PlayerCar::PlayerCar(int SelectedCar, sf::Texture& texture) : Car(sf::Vector2f(0, 0), 100, 450, GameObjects::Player, texture)
{
	setStats(SelectedCar);
	resetShotBullet();
	setPos(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 300));
	
	_AimLine.setFillColor(sf::Color::Black);
	_AimLine.setSize(sf::Vector2f(50.0f, 3.0f));
	_AimLine.setOrigin(0, _AimLine.getSize().y / 2.0f);

	if (_CrosshairTexture.loadFromFile("Resources/Texture/PlayerCar/crosshair.png")) {
		_AimCrosshair.setTexture(_CrosshairTexture);
		_AimCrosshair.setOrigin(_AimCrosshair.getLocalBounds().width / 2.0f, _AimCrosshair.getLocalBounds().height / 2.0f);
		_AimCrosshair.setScale(sf::Vector2f(0.15f, 0.15f));
	}
}


PlayerCar::~PlayerCar()
{

}

void PlayerCar::render(sf::RenderWindow& Window, bool renderCrosshair) {
	Window.draw(getSprite());
	if (renderCrosshair) {
		Window.draw(_AimLine);
		_AimCrosshair.setPosition(sf::Vector2f(sf::Mouse::getPosition(Window)));
		Window.draw(_AimCrosshair);
	}
}

void PlayerCar::handleEvent(sf::Event& Event)
{
	//Keyinput abfragen und in _Movement speichern
	_Movement = sf::Vector2f(0, 0);

	float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
	float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
	
	if (X > 50 || X < -50) {
		_Movement = sf::Vector2f(X / 100.0f, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		_Movement += sf::Vector2f(-1, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		_Movement += sf::Vector2f(1, 0);
	}
	else {
		_Movement = sf::Vector2f(0, 0);
	}

	if (Y > 50 || Y < -50) {
		if (Y < 0) {
			_Movement += sf::Vector2f(0, Y / 100.0f * 0.45);
		}
		else {
			_Movement += sf::Vector2f(0, Y / 100.0f * 1.2);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		_Movement += sf::Vector2f(0, -0.45);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		_Movement += sf::Vector2f(0, 1.2);
	}
	else {
		_Movement.y = 0;
	}

	if (Event.type == sf::Event::MouseMoved) {
		//Update angle on the _AimLine
		sf::Vector2f dir = sf::Vector2f(Event.mouseMove.x, Event.mouseMove.y) - getPos();
		float angle = std::atan(dir.y / dir.x) * 180.0f / PI;
		if (dir.x < 0) {
			_AimLine.setRotation(angle + 180);
		}
		else {
			_AimLine.setRotation(angle);
		}
	}
	else if (Event.type == sf::Event::JoystickMoved || 
		sf::Joystick::getAxisPosition(0, sf::Joystick::U) < -50 || sf::Joystick::getAxisPosition(0, sf::Joystick::U) > 50 || 
		sf::Joystick::getAxisPosition(0, sf::Joystick::R) < -50 || sf::Joystick::getAxisPosition(0, sf::Joystick::R) > 50) {
		float U = sf::Joystick::getAxisPosition(0, sf::Joystick::U);
		float R = sf::Joystick::getAxisPosition(0, sf::Joystick::R);
		if (U < -50 || U > 50 || R < -50 || R > 50) {
			sf::Vector2f dir = sf::Vector2f(U, R);
			float angle = std::atan(dir.y / dir.x) * 180.0f / PI;
			if (dir.x < 0) {
				_AimLine.setRotation(angle + 180);
			}
			else {
				_AimLine.setRotation(angle);
			}
		}
	}
	else if (Event.type == sf::Event::MouseButtonPressed || (Event.type == sf::Event::JoystickButtonPressed && sf::Joystick::isButtonPressed(0, 5))) {
		if (_Energy - 5 >= 10) {
			_Energy -= 5;
			//New approach seems to work fine
			_ShotBullet = _AimLine.getRotation();
			
			
			//Following code might be obsolete, if any errors occur please return to this bit, because it worked

			/*
			if (getPos().x > Event.mouseButton.x) {
				_ShotBullet = std::atanf((Event.mouseButton.y - getPos().y) / (Event.mouseButton.x - getPos().x)) * 180.0f / PI + 180;
			}
			else if (getPos().x < Event.mouseButton.x) {
				_ShotBullet = std::atanf((getPos().y - Event.mouseButton.y) / (getPos().x - Event.mouseButton.x)) * 180.0f / PI;
			}
			else {
				if (Event.mouseButton.y > getPos().y) {
					_ShotBullet = 90;
				}
				else {
					_ShotBullet = -90;
				}
			}*/

		}
	}
}

void PlayerCar::update(float FrameTime, int RoadSpeed)
{
	//_Movement anwenden - Car bewegen
	if (((getPos() + _Movement * FrameTime * (float)_Speed).x >= getWidth() / 2) && ((getPos() + _Movement * FrameTime * (float)_Speed).x <= SCREENWIDTH - getWidth() / 2)) {
		setPos(sf::Vector2f(getPos().x + (_Movement.x * _Speed * FrameTime), getPos().y));
	}

	if (getPos().y + getHeight() / 2 + _Movement.y * FrameTime * _Speed <= SCREENHEIGHT && getPos().y - getHeight() / 2 + _Movement.y * FrameTime * _Speed >= 0) {
		setPos(sf::Vector2f(getPos().x, getPos().y + _Movement.y * FrameTime * _Speed));
	}

	_AimLine.setPosition(getPos());

	//Energieverbrauch
	_Energy -= 2 * FrameTime;
}

bool PlayerCar::checkForCollision(GameObject * go)
{
	if (getSprite().getGlobalBounds().intersects(go->getSprite().getGlobalBounds())) {
		return true;
	}
	return false;
}

void PlayerCar::addHealth()
{
	if (_Health + 20 > _MaxHealth) {
		_Health = _MaxHealth;
	}
	else {
		_Health += 20;
	}
}

void PlayerCar::addEnergy()
{
	if (_Energy + 50 > _MaxEnergy) {
		_Energy = _MaxEnergy;
	}
	else {
		_Energy += 50;
	}
}

void PlayerCar::setStats(int id)
{
	std::vector<int> Stats = PlayerStats::getPlayerStats(id);
	_MaxHealth = Stats[0];
	_Health = _MaxHealth;
	_MaxEnergy = Stats[1];
	_Energy = _MaxEnergy;
	_Speed = Stats[2];
	_Bulletdamage = Stats[3];
}