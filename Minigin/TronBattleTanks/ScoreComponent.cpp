#include "ScoreComponent.h"
#include <iostream>

#include "HealthComponent.h"
#include "GameObject.h"

dae::ScoreComponent::ScoreComponent()
	: Component()
	, m_Score{ 0 }
{

}

void dae::ScoreComponent::Update()
{

}

void dae::ScoreComponent::Notify(Event currEvent, Subject* pSubject)
{
	//Score events
	if (currEvent == ObjectDied)
	{
		HealthComponent* health = static_cast<HealthComponent*>(pSubject);
		if (typeid(*health) == typeid(HealthComponent)) //TODO improve this
		{
			auto gb = health->GetOwner();
			if (gb == nullptr)
			{
				return;
			}

			auto gbScore = gb->GetComponent<ScoreComponent>();
			if (gbScore == nullptr)
			{
				return;
			}

			AddScore(gbScore->GetScore());

			std::cout << "Score updated: " << gbScore->GetScore() << std::endl;
		}
	}
}