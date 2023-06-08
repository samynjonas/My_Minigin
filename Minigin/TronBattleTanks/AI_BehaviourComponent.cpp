#include "AI_BehaviourComponent.h"

#include "TransformComponent.h"
#include "RenderComponent.h"

#include "CollisionManager.h"
#include "MiniginTimer.h"

#include "GameObject.h"

#include <vector>
#include <string>
#include <iostream>

dae::AI_BehaviourComponent::AI_BehaviourComponent()
{

}

dae::AI_BehaviourComponent::~AI_BehaviourComponent()
{

}

void dae::AI_BehaviourComponent::Initialize(const float& moveSpeed)
{
	m_MoveSpeed = moveSpeed;


}

void dae::AI_BehaviourComponent::Update()
{
	dae::RaycastInfo hit;
	auto raycastOrigin{ GetOwner()->transform()->GetWorldPosition() };

	if(CollisionManager::GetInstance().Raycast(raycastOrigin + GetOwner()->renderer()->GetTextureDimensions() / 2.f, Directions::Left, hit, 1000, { "Player" }))
	{
		//std::cout << "Raycast hit: " << hit.distance << std::endl;
	}
}