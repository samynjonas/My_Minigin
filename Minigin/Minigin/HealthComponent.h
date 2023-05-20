#pragma once
#include "Component.h"
#include "subject.h"

namespace dae
{
	class HealthComponent final : public Component, public subject
	{
	public:
		HealthComponent();
		~HealthComponent();

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void Update(float deltaTime) override;

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

				NotifyObservers(PlayerDied, this);
				
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
		void TakeLive(int amount)
		{
			m_Lives -= amount;
			if (m_Lives < 0)
			{
				m_Lives = 0;
			}
			else if (m_Lives > m_MaxLives)
			{
				m_Lives = m_MaxLives;
			}
		}

	private:
		int m_Health;
		int m_MaxHealth;

		int m_Lives;
		int m_MaxLives;

	};
}

