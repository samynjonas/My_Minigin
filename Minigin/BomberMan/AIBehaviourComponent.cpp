#include "AIBehaviourComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "RigidbodyComponent.h"

#include "CollisionManager.h"
#include "MiniginTimer.h"

#include "GameObject.h"

#include <vector>
#include <string>
#include <iostream>

dae::AIBehaviourComponent::AIBehaviourComponent()
	: Component()
{

}

dae::AIBehaviourComponent::~AIBehaviourComponent()
{

}

void dae::AIBehaviourComponent::Initialize(const float& moveSpeed, int smart)
{
	m_Smart = smart;
	m_MoveSpeed = moveSpeed;

	ChangeDirection();
}

void dae::AIBehaviourComponent::Update()
{
	//TODO add sight - when it sees player move towards him
	m_ElapsedSec += MiniginTimer::GetInstance().GetDeltaTime();
	if (m_ElapsedSec > move_till_change)
	{
		//TODO improve this - check also if there are options besides the same direction or reverse
		move_till_change += ORIGINAL_MOVE_TILL_CHANGE;
		//ChangeDirection();
	}
}

void dae::AIBehaviourComponent::Notify(Event currEvent, Subject* /*actor*/)
{
	if (currEvent == CollisionEnter)
	{
		ChangeDirection();
	}
}

void dae::AIBehaviourComponent::ChangeDirection()
{
	if (m_pRigidbody == nullptr)
	{
		m_pRigidbody = GetOwner()->GetComponent<RigidbodyComponent>();
		if (m_pRigidbody == nullptr)
		{
			return;
		}
	}

	constexpr int GRID_SIZE{ 32 };
	dae::RaycastInfo hit;
	const auto& raycastOrigin{ GetOwner()->transform()->GetWorldPosition() };

	std::vector<glm::vec2> directionOptions;
	//Check for available directions
	if (CollisionManager::GetInstance().Raycast({ raycastOrigin.x + GRID_SIZE / 2.f, raycastOrigin.y + GRID_SIZE / 2.f }, Directions::Above, hit, MAX_DISTANCE, 0, { "Walls" }) == false)
	{
		directionOptions.push_back({ 0, -1 });
	}
	if (CollisionManager::GetInstance().Raycast({ raycastOrigin.x + GRID_SIZE / 2.f, raycastOrigin.y + GRID_SIZE / 2.f }, Directions::Right, hit, MAX_DISTANCE, 0, { "Walls" }) == false)
	{
		directionOptions.push_back({ 1, 0 });
	}
	if (CollisionManager::GetInstance().Raycast({ raycastOrigin.x + GRID_SIZE / 2.f, raycastOrigin.y + GRID_SIZE / 2.f }, Directions::Left, hit, MAX_DISTANCE, 0, { "Walls" }) == false)
	{
		directionOptions.push_back({ -1, 0 });
	}
	if (CollisionManager::GetInstance().Raycast({ raycastOrigin.x + GRID_SIZE / 2.f, raycastOrigin.y + GRID_SIZE / 2.f }, Directions::Below, hit, MAX_DISTANCE, 0, { "Walls" }) == false)
	{
		directionOptions.push_back({ 0, 1 });
	}

	if (directionOptions.empty())
	{
		return;
	}


	glm::vec2 direction = directionOptions[rand() % directionOptions.size()];

	m_pRigidbody->ApplyForce(direction * m_MoveSpeed, RigidbodyComponent::ForceMode::Force);
}