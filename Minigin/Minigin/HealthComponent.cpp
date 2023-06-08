#include "HealthComponent.h"

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

void dae::HealthComponent::Update()
{

}