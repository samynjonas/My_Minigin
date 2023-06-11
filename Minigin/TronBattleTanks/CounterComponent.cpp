#include "CounterComponent.h"

dae::CounterComponent::CounterComponent()
	: Component()
{

}

void dae::CounterComponent::Initialize(int counterGoTo)
{
	m_CounterStop = counterGoTo;
	m_Count = 0;
}

void dae::CounterComponent::Notify(Event, subject*)
{
	++m_Count;
	if (m_Count >= m_CounterStop)
	{
		NotifyObservers(CounterFinished, this);
	}
}