#include "ActionCommands.h"
#include <iostream>
#include "GameObject.h"
#include "TransformComponent.h"

dae::MoveCommand::MoveCommand(GameObject* pGameObject, float moveSpeed)
	: Command(pGameObject)
	, m_MoveSpeed{ moveSpeed }
{
	m_pTransform = pGameObject->transform();
}

void dae::MoveCommand::Execute()
{
	glm::vec2 currPos{ m_pTransform->GetPosition() };
	currPos += m_AxisValue * m_MoveSpeed; //TODO add elapsedSec
	m_pTransform->SetPosition(currPos.x, currPos.y, 0);
}

void dae::MoveCommand::SetAxisValue(const glm::vec2& axisValue)
{
	m_AxisValue = axisValue;
}
