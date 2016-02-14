#pragma once

#include "stdafx.h"

struct PlayerStats
{
	static std::vector<int> getPlayerStats(int id)
	{
		int MaxHealth, MaxEnergy, Speed, Bulletdamage;
		switch (id)
		{
		case 0:
			MaxHealth = 50;
			MaxEnergy = 125;
			Speed = 500;
			Bulletdamage = 50;
			break;
		case 1:
			MaxHealth = 100;
			MaxEnergy = 50;
			Speed = 600;
			Bulletdamage = 75;
			break;
		case 2:
			MaxHealth = 75;
			MaxEnergy = 125;
			Speed = 350;
			Bulletdamage = 50;
			break;
		case 3:
			MaxHealth = 50;
			MaxEnergy = 50;
			Speed = 550;
			Bulletdamage = 100;
			break;
		case 4:
			MaxHealth = 200;
			MaxEnergy = 200;
			Speed = 250;
			Bulletdamage = 40;
			break;
		case 5:
			MaxHealth = 150;
			MaxEnergy = 125;
			Speed = 350;
			Bulletdamage = 50;
			break;
		default:
			MaxHealth = 100;
			MaxEnergy = 100;
			Speed = 100;
			Bulletdamage = 100;
			break;
		}

		return { MaxHealth, MaxEnergy, Speed, Bulletdamage };
	}
};