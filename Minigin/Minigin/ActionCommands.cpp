#include "ActionCommands.h"

#include "GameObject.h"

#include "TransformComponent.h"
#include "RigidbodyComponent.h"

#include "MiniginTimer.h"
#include "SceneManager.h"

#include <iostream>
#include <math.h>

dae::MoveCommand::MoveCommand(GameObject* pGameObject, float moveSpeed)
	: AnalogCommand(pGameObject)
	, m_MoveSpeed{ moveSpeed }
{

}
void dae::MoveCommand::Execute()
{
	if (m_pRigidBody == nullptr)
	{
		m_pRigidBody = GetGameObject()->GetComponent<RigidbodyComponent>();
		if (m_pRigidBody == nullptr)
		{
			return;
		}
	}

	glm::vec2 moveVector{ m_AxisValue * m_MoveSpeed * MiniginTimer::GetInstance().GetDeltaTime() };
	//std::cout << "moveVector: " << moveVector.x << " : " << moveVector.y << std::endl;

	m_pRigidBody->ApplyForce(moveVector, dae::RigidbodyComponent::ForceMode::Impulse);
}


dae::GridMoveCommand::GridMoveCommand(GameObject* pGameObject, float moveSpeed)
	: AnalogCommand(pGameObject)
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
		if (m_pRigidBody == nullptr)
		{
			return;
		}
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

	m_pRigidBody->ApplyForce(moveVector, dae::RigidbodyComponent::ForceMode::Force);
}

dae::RotationCommand::RotationCommand(GameObject* pGameObject, float rotationSpeed)
	: AnalogCommand(pGameObject)
	, m_RotationSpeed{ rotationSpeed }
{
	m_pTransform = pGameObject->transform();
}
void dae::RotationCommand::Execute()
{
	if (m_pTransform == nullptr)
	{
		m_pTransform = GetGameObject()->transform();
		if (m_pTransform == nullptr)
		{
			return;
		}
	}

	float angle{ 0.f };
	if (m_AxisValue.x == 0 && m_AxisValue.y == 0)
	{
		return;
	}
	if (m_AxisValue.x == 0)
	{
		if (m_AxisValue.y > 0)
		{
			angle = 90.f;
		}
		else
		{
			angle = -90.f;
		}
	}
	else if (m_AxisValue.y == 0)
	{
		if (m_AxisValue.x > 0)
		{
			angle = 0.f;
		}
		else
		{
			angle = 180.f;
		}
	}
	else
	{
		angle = atanf(m_AxisValue.y / m_AxisValue.x);
		angle *= 180.f / 3.14f; //TODO better PI

		if (m_AxisValue.x < 0)
		{
			angle += 180.f;
		}
	}
	m_pTransform->SetLocalRotation(angle);
}

dae::LoadSceneCommand::LoadSceneCommand(GameObject* pGameObject, const std::string sceneName)
	: Command(pGameObject)
	, m_SceneName{ sceneName }
{

}

void dae::LoadSceneCommand::Execute()
{
	SceneManager::GetInstance().LoadScene(m_SceneName);
}


dae::LoadNextSceneCommand::LoadNextSceneCommand(GameObject* pGameObject)
	: Command(pGameObject)
{

}

void dae::LoadNextSceneCommand::Execute()
{
	SceneManager& sceneManager = SceneManager::GetInstance();
	if (sceneManager.GetSceneNames().empty())
	{
		return;
	}
	
	int currentSceneId = sceneManager.StringToID(sceneManager.GetActiveSceneName(), false);
	if (currentSceneId == -1)
	{
		return;
	}

	++currentSceneId;
	if (currentSceneId >= sceneManager.GetSceneNames().size())
	{
		currentSceneId = 0;
	}

	SceneManager::GetInstance().LoadScene(sceneManager.GetSceneNames()[currentSceneId]);
}

dae::LoadPreviousSceneCommand::LoadPreviousSceneCommand(GameObject* pGameObject)
	: Command(pGameObject)
{

}

void dae::LoadPreviousSceneCommand::Execute()
{
	SceneManager& sceneManager = SceneManager::GetInstance();
	if (sceneManager.GetSceneNames().empty())
	{
		return;
	}

	int currentSceneId = sceneManager.StringToID(sceneManager.GetActiveSceneName(), false);
	if (currentSceneId == -1)
	{
		return;
	}

	--currentSceneId;
	if (currentSceneId < 0)
	{
		currentSceneId = static_cast<int>(sceneManager.GetSceneNames().size()) - 1;
	}

	SceneManager::GetInstance().LoadScene(sceneManager.GetSceneNames()[currentSceneId]);
}