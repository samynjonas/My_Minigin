#include "CounterComponent.h"
#include "MiniginTimer.h"

#include <iostream>

dae::CounterComponent::CounterComponent()
	: Component()
{

}

void dae::CounterComponent::Initialize(float counterGoTo)
{
	m_CounterStop = counterGoTo;
	m_Count = 0.f;

	m_IsFinished = false;
}

void dae::CounterComponent::Notify(Event, Subject*)
{
	if (m_Count >= m_CounterStop)
	{
		NotifyObservers(CounterFinished, this);
	}
}

void dae::CounterComponent::Update()
{
	if (m_IsFinished)
	{
		return;
	}

	if (m_Count >= m_CounterStop)
	{
		NotifyObservers(CounterFinished, this);
		m_IsFinished = true;
	}
	else
	{
		m_Count += MiniginTimer::GetInstance().GetDeltaTime();
	}
}