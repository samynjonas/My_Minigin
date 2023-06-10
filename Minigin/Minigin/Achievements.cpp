#include "Achievements.h"
#include <iostream>

#include "HealthComponent.h"

void dae::Achievements::Notify(Event event, subject*)
{
	if (event == ObjectDied)
	{
		std::cout << "player has died\n";
	}
}