#pragma once

#include "stdafx.h"

struct PlayerStats
{
	static std::vector<int> getPlayerStats(int id)
	{
		int MaxHealth, MaxEnergy, Speed, Bulletdamage;
		switch (id) //dmg 40, 50, 75, 80, 100
		{
		case 0:
			MaxHealth = 50;
			MaxEnergy = 100;
			Speed = 500;
			Bulletdamage = 50;
			break;
		case 1:
			MaxHealth = 100;
			MaxEnergy = 50;
			Speed = 600;
			Bulletdamage = 50;
			break;
		case 2:
			MaxHealth = 100;
			MaxEnergy = 75;
			Speed = 400;
			Bulletdamage = 75;
			break;
		case 3:
			MaxHealth = 40;
			MaxEnergy = 50;
			Speed = 600;
			Bulletdamage = 100;
			break;
		case 4:
			MaxHealth = 175;
			MaxEnergy = 150;
			Speed = 250;
			Bulletdamage = 40;
			break;
		case 5:
			MaxHealth = 100;
			MaxEnergy = 150;
			Speed = 350;
			Bulletdamage = 40;
			break;
		default:
			return {};
			break;
		}

		return { MaxHealth, MaxEnergy, Speed, Bulletdamage };
	}

	static std::vector<int> getMaxStats()
	{
		std::vector<int> CurrentStats = getPlayerStats(0);
		std::vector<int> MaxStats = { 0, 0, 0, 0 };

		unsigned int i = 0;
		while (CurrentStats.size() > 0)
		{
			for (unsigned int j = 0; j < MaxStats.size(); j++)
			{
				MaxStats[j] = std::max(MaxStats[j], CurrentStats[j]);
			}

			i++;
			CurrentStats = getPlayerStats(i);
		}

		return MaxStats;
	}
};