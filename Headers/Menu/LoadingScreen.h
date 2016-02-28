#pragma once
#include "Menu/Menu.h"

class LoadingScreen : public Menu
{
public:
	LoadingScreen();
	~LoadingScreen();

	void render(sf::RenderWindow& Window);
	GameState handleEvents(sf::RenderWindow& Window);

	bool isFadingAway() { return _IsFadingAway; }
	void fadeAway();
	bool isDoneFading();

private:
	bool _IsFadingAway;
	sf::Clock _FadeTimer;
	float _FadeTime;

	sf::RectangleShape _Background;
};

