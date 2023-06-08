#include "AI_BehaviourComponent.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "RigidbodyComponent.h"

#include "CollisionManager.h"
#include "MiniginTimer.h"

#include "GameObject.h"

#include <vector>
#include <string>
#include <iostream>

dae::AI_BehaviourComponent::AI_BehaviourComponent()
	: Component()
{

}

dae::AI_BehaviourComponent::~AI_BehaviourComponent()
{

}

void dae::AI_BehaviourComponent::Initialize(const float& moveSpeed)
{
	m_MoveSpeed = moveSpeed;
	ChangeDirection();
}

void dae::AI_BehaviourComponent::Update()
{
	dae::RaycastInfo hit;
	auto raycastOrigin{ GetOwner()->transform()->GetWorldPosition() };

	//if(CollisionManager::GetInstance().Raycast(raycastOrigin + GetOwner()->renderer()->GetTextureDimensions() / 2.f, Directions::Above, hit, 750, { "Player" }))
	//{
	//	std::cout << "Raycast hit: " << hit.distance << std::endl;
	//}
	//if (CollisionManager::GetInstance().Raycast(raycastOrigin + GetOwner()->renderer()->GetTextureDimensions() / 2.f, Directions::Right, hit, 1000, { "Player" }))
	//{
	//	//std::cout << "Raycast hit: " << hit.distance << std::endl;
	//}
	//if (CollisionManager::GetInstance().Raycast(raycastOrigin + GetOwner()->renderer()->GetTextureDimensions() / 2.f, Directions::Above, hit, 1000, { "Player" }))
	//{
	//	//std::cout << "Raycast hit: " << hit.distance << std::endl;
	//}
	//if (CollisionManager::GetInstance().Raycast(raycastOrigin + GetOwner()->renderer()->GetTextureDimensions() / 2.f, Directions::Below, hit, 1000, { "Player" }))
	//{
	//	//std::cout << "Raycast hit: " << hit.distance << std::endl;
	//}
}

void dae::AI_BehaviourComponent::ChangeDirection()
{
	//TODO will be called on collision
	if (m_pRigidbody == nullptr)
	{
		m_pRigidbody = GetOwner()->GetComponent<RigidbodyComponent>();
		if (m_pRigidbody == nullptr)
		{
			return;
		}
	}

	dae::RaycastInfo hit;
	auto raycastOrigin{ GetOwner()->transform()->GetWorldPosition() };

	std::vector<glm::vec2> directionOptions;
	//Check for available directions
	if(CollisionManager::GetInstance().Raycast(raycastOrigin + GetOwner()->renderer()->GetTextureDimensions() / 2.f, Directions::Above, hit, 100, { "Walls" }))
	{
		directionOptions.push_back({ 0, -1 });
	}
	if (CollisionManager::GetInstance().Raycast(raycastOrigin + GetOwner()->renderer()->GetTextureDimensions() / 2.f, Directions::Right, hit, 100, { "Walls" }))
	{
		directionOptions.push_back({ 1, 0 });
	}
	if (CollisionManager::GetInstance().Raycast(raycastOrigin + GetOwner()->renderer()->GetTextureDimensions() / 2.f, Directions::Left, hit, 100, { "Walls" }))
	{
		directionOptions.push_back({ -1, 0 });
	}
	if (CollisionManager::GetInstance().Raycast(raycastOrigin + GetOwner()->renderer()->GetTextureDimensions() / 2.f, Directions::Below, hit, 100, { "Walls" }))
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