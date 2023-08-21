#pragma once
#include "Component.h"
#include "subject.h"
#include "Observer.h"

namespace dae
{
	class CounterComponent final : public Component, public Subject, public Observer
	{
	public:
		CounterComponent();
		~CounterComponent() = default;

		CounterComponent(const CounterComponent& other) = delete;
		CounterComponent(CounterComponent&& other) = delete;
		CounterComponent& operator=(const CounterComponent& other) = delete;
		CounterComponent& operator=(CounterComponent&& other) = delete;

		void Initialize(int counterGoTo);
		void Update() override {}

		void Notify(Event currEvent, Subject* actor);

	private:
		int m_CounterStop{ 1 };
		int m_Count{ 0 };

	};
}

