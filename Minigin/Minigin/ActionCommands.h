#pragma once
#include "Command.h"
#include "glm/glm.hpp"

namespace dae
{
	class GameObject;
	class TransformComponent;
	class RigidbodyComponent;
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

		TransformComponent* m_pTransform{};

	};

	class GridMoveCommand final : public Command
	{
	public:
		GridMoveCommand(GameObject* pGameObject, float moveSpeed);
		~GridMoveCommand() = default;

		GridMoveCommand(const GridMoveCommand& other) = delete;
		GridMoveCommand(GridMoveCommand&& other) = delete;
		GridMoveCommand& operator=(const GridMoveCommand& other) = delete;
		GridMoveCommand& operator=(GridMoveCommand&& other) = delete;

		void Execute() override;
		void SetAxisValue(const glm::vec2& axisValue);

	private:
		const float m_MoveSpeed;
		glm::vec2 m_AxisValue{ 0.f, 0.f };

		RigidbodyComponent* m_pRigidBody{};

	};

	class RotationCommand final : public Command
	{
	public:
		RotationCommand(GameObject* pGameObject, float rotationSpeed);
		~RotationCommand() = default;

		RotationCommand(const RotationCommand& other) = delete;
		RotationCommand(RotationCommand&& other) = delete;
		RotationCommand& operator=(const RotationCommand& other) = delete;
		RotationCommand& operator=(RotationCommand&& other) = delete;

		void Execute() override;
		void SetAxisValue(const glm::vec2& axisValue);

	private:
		const float m_RotationSpeed;
		glm::vec2 m_AxisValue{ 0.f, 0.f };

		TransformComponent* m_pTransform{};
	};

}

