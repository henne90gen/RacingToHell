#include "stdafx.h"
#include "GameObject\PlayerCar.h"


PlayerCar::PlayerCar(unsigned int id, int selectedCar, sf::Texture& texture) : 
	Car(id, sf::Vector2f(0, 0), 100, 500, GameObjectType::Player, texture), 
	_CrosshairSpeed(600.0f), _SelectedCar(selectedCar), _AccelerationTime(0.1f)
{
	setStats(_SelectedCar);
	resetShotBullet();
	setPos(sf::Vector2f(SCREENWIDTH / 2, SCREENHEIGHT - 300));
	
	_AimLine.setFillColor(sf::Color::Black);
	_AimLine.setSize(sf::Vector2f(50.0f, 3.0f));
	_AimLine.setOrigin(0, _AimLine.getSize().y / 2.0f);

	if (_CrosshairTexture.loadFromFile("Resources/Texture/PlayerCar/crosshair.png")) {
		_Crosshair.setTexture(_CrosshairTexture);
		_Crosshair.setOrigin(_Crosshair.getLocalBounds().width / 2.0f, _Crosshair.getLocalBounds().height / 2.0f);
		_Crosshair.setScale(sf::Vector2f(0.15f, 0.15f));
		_Crosshair.setPosition(SCREENWIDTH / 2.0f, SCREENHEIGHT / 2.0f);
	}

	sf::Listener::setDirection(0.f, 0.f, -1.f);
}

PlayerCar::PlayerCar(std::istream& stream, std::vector<std::shared_ptr<sf::Texture>>& textures) : Car(stream, GameObjectType::Player)
{
	PlayerCar::operator<<(stream);
	setStats(_SelectedCar);
	initTexture((*textures.at(_SelectedCar)));
}

void PlayerCar::render(sf::RenderWindow& Window, bool renderCrosshair) {
	Window.draw(getSprite());
	if (renderCrosshair) {
		Window.draw(_AimLine);
		Window.draw(_Crosshair);
	}
}

void PlayerCar::handleEvent(sf::Event& Event)
{
	// Apply key input to car
	_Movement = sf::Vector2f(0, 0);
	float X = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
	float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
	
	if (X > 50 || X < -50) {
		_Movement += sf::Vector2f(X / 100.0f, 0);
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
	
	// Apply mouse and joystick movement
	_CrosshairMovement = sf::Vector2f(0, 0);
	if (Event.type == sf::Event::MouseMoved) {
		_Crosshair.setPosition(Event.mouseMove.x, Event.mouseMove.y);
	}
	else if (Event.type == sf::Event::JoystickMoved) {
		float U = sf::Joystick::getAxisPosition(0, sf::Joystick::U);
		float R = sf::Joystick::getAxisPosition(0, sf::Joystick::R);
		if (U < -50 || U > 50 || R < -50 || R > 50) {
			_CrosshairMovement += sf::Vector2f(U / 65.0f, R / 65.0f);
		}
	}
	else if (Event.type == sf::Event::MouseButtonPressed || (Event.type == sf::Event::JoystickButtonPressed && sf::Joystick::isButtonPressed(0, 5))) {
		if (_Energy - 5 >= 10) {
			_Energy -= 5;
			_ShotBullet = _Crosshair.getPosition() - getPos();
			_ShotBullet = _ShotBullet / std::sqrtf(std::powf(_ShotBullet.x, 2) + std::powf(_ShotBullet.y, 2));
		}
	}
}

void PlayerCar::update(float FrameTime, int RoadSpeed)
{
	Car::update(FrameTime, RoadSpeed);

	if (_Movement != sf::Vector2f(0, 0))
	{
		if (_Acceleration + FrameTime / _AccelerationTime <= 1.0f)
		{
			_Acceleration += FrameTime / _AccelerationTime;
		}
		else
		{
			_Acceleration = 1.0f;
		}
	}
	else
	{
		_Acceleration = 0.0f;
	}

	//_Movement anwenden - Car bewegen
	if (((getPos() + _Movement * FrameTime * (float)_Speed).x * _Acceleration >= getWidth() / 2) && ((getPos() + _Movement * FrameTime * (float)_Speed).x * _Acceleration <= SCREENWIDTH - getWidth() / 2)) {
		setPos(sf::Vector2f(getPos().x + _Movement.x * _Speed * _Acceleration * FrameTime, getPos().y));
	}

	if (getPos().y + getHeight() / 2 + _Movement.y * FrameTime * _Speed * _Acceleration <= SCREENHEIGHT && getPos().y - getHeight() / 2 + _Movement.y * FrameTime * _Speed * _Acceleration >= 0) {
		setPos(sf::Vector2f(getPos().x, getPos().y + _Movement.y * FrameTime * _Speed * _Acceleration));
	}

	//Update _AimLine
	_AimLine.setPosition(getPos());
	sf::Vector2f dir = _Crosshair.getPosition() - getPos();
	float angle = std::atan(dir.y / dir.x) * 180.0f / PI;
	if (dir.x < 0) {
		_AimLine.setRotation(angle + 180);
	}
	else {
		_AimLine.setRotation(angle);
	}

	_Crosshair.setPosition(_Crosshair.getPosition() + _CrosshairMovement * FrameTime * _CrosshairSpeed);

	// Sound listener
	sf::Listener::setPosition(getPos().x, 0.f, getPos().y);
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

void PlayerCar::operator>>(std::ostream& stream)
{
	Car::operator>>(stream);
	write(stream, _SelectedCar);
}

void PlayerCar::operator<<(std::istream& stream)
{
	read(stream, _SelectedCar);
}