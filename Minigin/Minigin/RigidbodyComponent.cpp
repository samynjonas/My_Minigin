#include "RigidbodyComponent.h"

#include "GameObject.h"

#include "TransformComponent.h"
#include "BoxColliderComponent.h"

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
				//TODO fix calculation

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

void dae::RigidbodyComponent::Notify(Event currEvent, subject* pSubject)
{
	if (currEvent == CollisionEnter)
	{
		//Check type of rigidbody - bounce, stop
		
		if (m_PhysicsMaterial.bounciness == 0)
		{
			m_ActiveForceMode = ForceMode::None;
			m_CollidingVelocity = -m_Velocity;
		}
		
		BoxColliderComponent* collider = static_cast<BoxColliderComponent*>(pSubject);
		if (typeid(*collider) == typeid(BoxColliderComponent)) //TODO improve this
		{
			//Has bounced against collider
			glm::vec2 collPos{ collider->GetRect()->_x, collider->GetRect()->_y };
			glm::vec2 rbPos{ collider->GetCollisionPoint() };

			int checkPosX{};
			if (rbPos.x < collPos.x + collider->GetRect()->_width / 2)
			{
				//Left side
				checkPosX = static_cast<int>(collPos.x);
			}
			else
			{
				//Right side
				checkPosX = collider->GetRect()->maxX();
			}

			int checkPosY{};
			if (rbPos.y < collPos.y + collider->GetRect()->_height / 2.f)
			{
				//Top side
				checkPosY = static_cast<int>(collPos.y);
			}
			else
			{
				//Bottom side
				checkPosY = collider->GetRect()->maxY();
			}

			if (abs(rbPos.x - checkPosX) < abs(rbPos.y - checkPosY))
			{
				m_Velocity.x *= -m_PhysicsMaterial.bounciness;
				//m_CollidingVelocity.y = -m_Velocity.y;
			}
			else
			{
				m_Velocity.y *= -m_PhysicsMaterial.bounciness;
				//m_CollidingVelocity.x = -m_Velocity.x;
			}
		}
		else
		{
			m_Velocity *= -m_PhysicsMaterial.bounciness;
			m_CollidingVelocity = -m_Velocity;
		}

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