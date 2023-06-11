#pragma once
#include "glm/glm.hpp"

namespace dae
{
	class GameObject;

	class Command
	{
	public:
		explicit Command(GameObject* pGameObject) 
			: m_pGameObject{ pGameObject } 
		{
		
		};

		virtual ~Command() = default;
		virtual void Execute() = 0;
		
		GameObject* GetGameObject() const
		{
			return m_pGameObject;
		}

	private:
		GameObject* m_pGameObject{ nullptr };

	};

	class AnalogCommand : public Command
	{
	public:

		explicit AnalogCommand(GameObject* pGameObject)
			: Command{ pGameObject }
		{

		};

		virtual void Execute() = 0;
		virtual void SetAxix(glm::vec2 analogInput)
		{
			m_AxisValue = analogInput;
		}

	protected:
		glm::vec2 m_AxisValue{ 0.f, 0.f };

	private:


	};
}


