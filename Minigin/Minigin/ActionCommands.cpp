#include "ActionCommands.h"
#include <iostream>

#include "GameObject.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "MiniginTimer.h"

dae::MoveCommand::MoveCommand(GameObject* pGameObject, float moveSpeed)
	: Command(pGameObject)
	, m_MoveSpeed{ moveSpeed }
{
	m_pTransform = pGameObject->transform();
}

void dae::MoveCommand::Execute()
{
	glm::vec2 moveVector{ m_AxisValue * m_MoveSpeed * MiniginTimer::GetInstance().GetDeltaTime() };
	if (m_pTransform)
	{
		m_pTransform->Translate(moveVector);
	}
}

void dae::MoveCommand::SetAxisValue(const glm::vec2& axisValue)
{
	m_AxisValue = axisValue;
}


dae::GridMoveCommand::GridMoveCommand(GameObject* pGameObject, float moveSpeed)
	: Command(pGameObject)
	, m_MoveSpeed{ moveSpeed }
{

}

void dae::GridMoveCommand::Execute()
{
	if (m_AxisValue.x == 0 && m_AxisValue.y == 0)
	{
		return;
	}
	if (m_pRigidBody == nullptr)
	{
		m_pRigidBody = GetGameObject()->GetComponent<RigidbodyComponent>();
	}

	glm::vec2 moveVector{ m_MoveSpeed, m_MoveSpeed };
	if (m_AxisValue.x < 0)
	{
		moveVector.x *= -1;
	}
	else if (m_AxisValue.x == 0)
	{
		moveVector.x = 0;
	}
	
	if (m_AxisValue.y < 0)
	{
		moveVector.y *= -1;
	}
	else if (m_AxisValue.y == 0)
	{
		moveVector.y = 0;
	}

	if (m_pRigidBody)
	{
		m_pRigidBody->ApplyForce(moveVector, dae::RigidbodyComponent::ForceMode::Force);
	}
}

void dae::GridMoveCommand::SetAxisValue(const glm::vec2& axisValue)
{
	m_AxisValue = axisValue;
}
