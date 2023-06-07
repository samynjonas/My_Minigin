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
				m_Velocity.x += m_PhysicsMaterial.dynamicFriction * sign;

				if (m_Velocity.y > 0)
				{
					sign = 1;
				}
				m_Velocity.y += m_PhysicsMaterial.dynamicFriction * sign;

			}
			
			GetOwner()->transform()->Translate(m_Velocity * MiniginTimer::GetInstance().GetDeltaTime(), true);
		}
	}

	if (m_IsColliding)
	{
		//Move out of collider
		GetOwner()->transform()->Translate(m_CollidingVelocity * MiniginTimer::GetInstance().GetDeltaTime(), true);
	}
}

void dae::RigidbodyComponent::ApplyForce(const glm::vec2& vector, ForceMode mode)
{
	if (m_IsColliding)
	{
		return;
	}

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

void dae::RigidbodyComponent::Notify(Event currEvent, subject*)
{
	if (currEvent == CollisionEnter)
	{
		//Check type of rigidbody - bounce, stop
		m_CollidingVelocity = -m_Velocity;
		m_Velocity *= -m_PhysicsMaterial.bounciness;

		m_IsColliding = true;
	}
	else if (currEvent == CollisionExit)
	{
		m_IsColliding = false;
		m_CollidingVelocity = { 0.f, 0.f };
	}
}

void dae::RigidbodyComponent::SetPhysicsMaterial(const dae::PhysicsMaterial& physicsMaterial)
{
	m_PhysicsMaterial = physicsMaterial;
}