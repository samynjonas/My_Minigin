#include "AI_BehaviourComponent.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "RigidbodyComponent.h"
#include "GunComponent.h"

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
	if (m_pGunComponent == nullptr)
	{
		m_pGunComponent = GetOwner()->GetComponent<GunComponent>();
		if (m_pGunComponent == nullptr)
		{
			m_pGunComponent = GetOwner()->GetComponentInChildren<GunComponent>();
			if (m_pGunComponent == nullptr)
			{
				return;
			}
		}
	}

	if (m_pGunComponent->IsInCooldown())
	{
		return;
	}

	dae::RaycastInfo hit{};
	const auto& raycastOrigin{ GetOwner()->transform()->GetWorldPosition() };
	glm::vec2 halfTexture = GetOwner()->renderer()->GetTextureDimensions() / 2.f;


	//TODO Improve so it wont check all the colliders multiple timer
	//TODO when a tank sees a player move towards them, only when he doesnt see him anymore go random again
	if (CollisionManager::GetInstance().Raycast(raycastOrigin + halfTexture, Directions::Left, hit, 750, static_cast<int>(halfTexture.x), { "Player" }))
	{
		AimAndShoot({ -1, 0 });
	}

	if (CollisionManager::GetInstance().Raycast(raycastOrigin + halfTexture, Directions::Right, hit, 750, static_cast<int>(halfTexture.x), { "Player" }))
	{
		AimAndShoot({ 1, 0 });
	}

	if (CollisionManager::GetInstance().Raycast(raycastOrigin + halfTexture, Directions::Above, hit, 750, static_cast<int>(halfTexture.y), { "Player" }))
	{
		AimAndShoot({ 0, -1 });
	}

	if (CollisionManager::GetInstance().Raycast(raycastOrigin + halfTexture, Directions::Below, hit, 750, static_cast<int>(halfTexture.y), { "Player" }))
	{
		AimAndShoot({ 0, 1 });
	}

	m_ElapsedSec += MiniginTimer::GetInstance().GetDeltaTime();
	if (m_ElapsedSec > move_till_change)
	{
		//TODO improve this - check also if there are options besides the same direction or reverse
		move_till_change += ORIGINAL_MOVE_TILL_CHANGE;
		ChangeDirection();
	}
}

void dae::AI_BehaviourComponent::Notify(Event currEvent, subject* /*actor*/)
{
	if (currEvent == CollisionExit)
	{
		ChangeDirection();
	}
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
	const auto& raycastOrigin{ GetOwner()->transform()->GetWorldPosition() };

	std::vector<glm::vec2> directionOptions;
	//Check for available directions
	if (CollisionManager::GetInstance().Raycast({ raycastOrigin.x + GetOwner()->renderer()->GetTextureDimensions().x / 2.f, raycastOrigin.y + GetOwner()->renderer()->GetTextureDimensions().x / 2.f }, Directions::Above, hit, MAX_DISTANCE, 0, { "Walls" }) == false)
	{																													
		directionOptions.push_back({ 0, -1 });																			
	}																													
	if (CollisionManager::GetInstance().Raycast({ raycastOrigin.x + GetOwner()->renderer()->GetTextureDimensions().x / 2.f, raycastOrigin.y + GetOwner()->renderer()->GetTextureDimensions().y / 2.f }, Directions::Right, hit, MAX_DISTANCE, 0, { "Walls" }) == false)
	{																													
		directionOptions.push_back({ 1, 0 });																			
	}																													
	if (CollisionManager::GetInstance().Raycast({ raycastOrigin.x + GetOwner()->renderer()->GetTextureDimensions().x / 2.f, raycastOrigin.y + GetOwner()->renderer()->GetTextureDimensions().y / 2.f }, Directions::Left,  hit, MAX_DISTANCE, 0, { "Walls" }) == false)
	{																													
		directionOptions.push_back({ -1, 0 });																			
	}																													
	if (CollisionManager::GetInstance().Raycast({ raycastOrigin.x + GetOwner()->renderer()->GetTextureDimensions().x / 2.f, raycastOrigin.y + GetOwner()->renderer()->GetTextureDimensions().y / 2.f }, Directions::Below, hit, MAX_DISTANCE, 0, { "Walls" }) == false)
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
void dae::AI_BehaviourComponent::AimAndShoot(const glm::vec2& direction)
{
	float angle{};
	if (direction.x == 0)
	{
		if (direction.y > 0)
		{
			angle = 90.f;
		}
		else
		{
			angle = -90.f;
		}
	}
	else if (direction.y == 0)
	{
		if (direction.x > 0)
		{
			angle = 0.f;
		}
		else
		{
			angle = 180.f;
		}
	}
	GetOwner()->transform()->SetLocalRotation(angle);
	m_pGunComponent->Fire();
}