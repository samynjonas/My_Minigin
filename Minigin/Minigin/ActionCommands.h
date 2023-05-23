#pragma once
#include "Command.h"
#include "glm/glm.hpp"

namespace dae
{
	class GameObject;
	class TransformComponent;
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* pGameObject, float moveSpeed);
		~MoveCommand() = default;

		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) = delete;
		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand& operator=(MoveCommand&& other) = delete;

		void Execute() override;
		void SetAxisValue(const glm::vec2& axisValue);

	private:
		const float m_MoveSpeed;
		glm::vec2 m_AxisValue{ 0.f, 0.f };

		TransformComponent* m_pTransform;

	};

}
