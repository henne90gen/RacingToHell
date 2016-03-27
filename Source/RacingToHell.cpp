// RacingToHell.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Framework.hpp"

//int main()
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	Framework _Framework;
	_Framework.run();

	return 0;
}