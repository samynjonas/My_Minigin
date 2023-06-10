#include "HealthComponent.h"
#include "GameObject.h"
#include "BoxColliderComponent.h"
#include <iostream>

dae::HealthComponent::HealthComponent()
	: Component()
	, m_Health{ 0 }
	, m_MaxHealth{ 0 }
	, m_Lives{ 1 }
	, m_MaxLives{ 1 }
{

}

dae::HealthComponent::~HealthComponent()
{

}

void dae::HealthComponent::Initialize(int MaxHealt, int MaxLives)
{
	SetMaxHealth(MaxHealt);
	SetMaxLives(MaxLives);
}

void dae::HealthComponent::Notify(Event currEvent, subject*)
{
	//This needs to be better
	if (currEvent == CollisionEnter)
	{
		Damage(1);
	}

	if (currEvent == TriggerEnter)
	{
		TakeLive(1);
	}
}

void dae::HealthComponent::TakeLive(int amount)
{
	m_Lives -= amount;
	if (m_Lives < 0)
	{
		m_Lives = 0;
		NotifyObservers(ObjectDied, this);

		GetOwner()->MarkForDead();

	}
	else if (m_Lives > m_MaxLives)
	{
		m_Lives = m_MaxLives;
	}
}
