#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent()
	: Component()
	, m_Score{ 0 }
{

}

void dae::ScoreComponent::Update(float)
{

}

void dae::ScoreComponent::Notify(Event, subject*)
{
	//Score events
}