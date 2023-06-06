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

void dae::HealthComponent::Update()
{

}

