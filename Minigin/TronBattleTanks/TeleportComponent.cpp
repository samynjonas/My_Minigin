#include "TeleportComponent.h"

#include "glm/glm.hpp"

#include "GameObject.h"
#include "TransformComponent.h"

#include <iostream>

dae::TeleportComponent::TeleportComponent()
	: Component()
{

}

dae::TeleportComponent::~TeleportComponent()
{

}

void dae::TeleportComponent::Initialize(rect mapRect)
{
	m_MapRect = mapRect;
}

void dae::TeleportComponent::Update()
{

}

void dae::TeleportComponent::Notify(Event currEvent, Subject*)
{
	if (currEvent == TriggerEnter)
	{
		TeleportToRandomPosition();
	}
}

void dae::TeleportComponent::TeleportToRandomPosition()
{
	if (m_MapRect.width <= 0 || m_MapRect.height <= 0)
	{
		return;
	}

	int randomX = rand() % m_MapRect.width + m_MapRect.x;
	int randomY = rand() % m_MapRect.height + m_MapRect.y;

	std::cout << "TELEPORT to: " << randomX << " : " << randomY << std::endl;

	GetOwner()->transform()->SetLocalPosition({ randomX, randomY });


}