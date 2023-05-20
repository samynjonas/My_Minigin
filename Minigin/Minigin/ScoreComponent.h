#pragma once
#include "Component.h"
#include "subject.h"

namespace dae
{
	class ScoreComponent final : public Component, public subject, public Observer
	{
	public:
		ScoreComponent();
		~ScoreComponent() = default;

		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		void Update(float) override;

		void AddScore(int amount)
		{
			m_Score += amount;

			NotifyObservers(ScoreUpdated, this);
		}

		virtual void Notify(Event currEvent, subject* actor) override; //TODO make subject better, not hardcoding the type

		int GetScore() const
		{
			return m_Score;
		}


	private:
		int m_Score;

	};
}



