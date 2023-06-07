#pragma once
#include "Singleton.h"

namespace dae
{
	class MiniginTimer final : public Singleton<MiniginTimer>
	{
	public:
		void SetDeltaTime(float deltaTime)
		{
			m_PrevDeltaTime = m_DeltaTime;
			m_DeltaTime = deltaTime;
		}
		float GetDeltaTime() const
		{
			return m_DeltaTime;
		}
		float GetPrevDeltaTime() const
		{
			return m_PrevDeltaTime;
		}

	private:
		friend class Singleton<MiniginTimer>;
		MiniginTimer() = default;

		float m_DeltaTime{};
		float m_PrevDeltaTime{};

	};
}

