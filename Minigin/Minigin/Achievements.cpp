#include "Achievements.h"
#include <iostream>

void dae::Achievements::Notify(Event event, Subject*)
{
	if (event == ObjectDied)
	{
		std::cout << "player has died\n";
	}
}