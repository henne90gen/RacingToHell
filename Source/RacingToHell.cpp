// RacingToHell.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Framework.hpp"

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	Framework _Framework;
	_Framework.run();

	return 0;
}

int main() 
{
	Framework _Framework;
	_Framework.run();

	return 0;
}