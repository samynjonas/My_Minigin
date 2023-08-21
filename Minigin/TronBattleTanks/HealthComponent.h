#pragma once
#include "Component.h"
#include "Subject.h"
#include "Observer.h"

namespace dae
{
	class HealthComponent final : public Component, public Subject, public Observer
	{
	public:
		HealthComponent();
		~HealthComponent();

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void Initialize(int MaxHealt, int MaxLives);
		void Update() override {};

		void SetMaxHealth(int amount, bool setHealth = true)
		{
			m_MaxHealth = amount;
			if (setHealth)
			{
				m_Health = amount;
			}
		}

		void SetHealth(int amount, bool overrideMax = false)
		{
			m_Health = amount;
			if (overrideMax == false)
			{
				if (m_Health > m_MaxHealth)
				{
					m_Health = m_MaxHealth;
				}
			}
		}

		void Damage(int amount)
		{
			m_Health -= amount;
			if (m_Health < amount)
			{
				m_Health = 0;

				NotifyObservers(LiveLost, this);

				TakeLive(1);				
			}
			else if (m_Health > m_MaxHealth)
			{
				m_Health = m_MaxHealth;
			}
		}

		int GetMaxHealth() const
		{
			return m_MaxHealth;
		}
		int GetHealth() const
		{
			return m_Health;
		}

		bool IsDead() const
		{
			if (m_Health <= 0 && m_Lives <= 0)
			{
				return true;
			}
			return false;
		}

		void SetMaxLives(int amount, bool setLives = true)
		{
			m_MaxLives = amount;
			if (setLives)
			{
				m_Lives = amount;
			}
		}
		void SetLives(int amount, bool overrideMax = false)
		{
			m_Lives = amount;
			if (overrideMax == false)
			{
				if (m_Lives > m_MaxLives)
				{
					m_Lives = m_MaxLives;
				}
			}
		}
		int GetLives() const
		{
			return m_Lives;
		}
		void TakeLive(int amount);

		void Notify(Event currEvent, Subject* actor);

	private:
		int m_Health{ 1 };
		int m_MaxHealth{ 1 };

		int m_Lives{ 1 };
		int m_MaxLives{ 1 };

	};
}

