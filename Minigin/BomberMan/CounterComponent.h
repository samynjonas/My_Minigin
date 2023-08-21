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

		void Initialize(float counterGoTo);
		void Update() override;

		void Notify(Event currEvent, Subject* actor);

		int GetCurrentCount() const
		{
			return static_cast<int>(m_Count);
		}

		int GetTimeToGo() const
		{
			return static_cast<int>(m_CounterStop - m_Count);
		}

	private:
		float m_CounterStop{ 1 };
		float m_Count{ 0 };

		bool m_IsFinished{ false };
	};
}
