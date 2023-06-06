#include "RigidbodyComponent.h"

#include "GameObject.h"

#include "TransformComponent.h"

#include "MiniginTimer.h"

#include <iostream>

dae::RigidbodyComponent::RigidbodyComponent()
	: Component()
{

}

dae::RigidbodyComponent::~RigidbodyComponent()
{

}

void dae::RigidbodyComponent::Update()
{
	if (m_ActiveForceMode != ForceMode::None)
	{
		if (m_Velocity.x != 0.f || m_Velocity.y != 0.f)
		{
			if (m_ActiveForceMode == ForceMode::Impulse)
			{
				//TODO this calculation isnt correct

				//Decrease velocity
				float sign{ 1 };
				if (m_Velocity.x < 0)
				{
					sign = -1;
				}
				m_Velocity.x += DRAG * sign;

				if (m_Velocity.y > 0)
				{
					sign = 1;
				}
				m_Velocity.y += DRAG * sign;

			}
			
			//Add velocity to transform -- //Is not very effecient, other way?
			GetOwner()->transform()->Translate(m_Velocity * MiniginTimer::GetInstance().GetDeltaTime(), true);
		}
	}
}

void dae::RigidbodyComponent::ApplyForce(const glm::vec2& vector, ForceMode mode)
{
	m_ActiveForceMode = mode;
	m_Velocity = vector;
}

void dae::RigidbodyComponent::SetUseGravity(bool state)
{
	m_UseGravity = state;
}
void dae::RigidbodyComponent::SetMass(float mass)
{
	m_Mass = mass;
}