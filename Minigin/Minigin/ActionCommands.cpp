#include "ActionCommands.h"
#include <iostream>

dae::MoveCommand::MoveCommand(GameObject* pGameObject)
	: Command(pGameObject)
{

}

void dae::MoveCommand::Execute()
{
	std::cout << "Move" << std::endl;
}