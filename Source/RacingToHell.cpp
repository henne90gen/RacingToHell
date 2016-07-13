// RacingToHell.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Framework.hpp"

#ifdef SFML_SYSTEM_WINDOWS
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	main();
}
#endif

int main()
{
	Framework _Framework;
	_Framework.run();

	return 0;
}
