#pragma once
#include "Command.h"
#include "glm/glm.hpp"

#include <string>

namespace dae
{
	class GameObject;
	class TransformComponent;
	class RigidbodyComponent;
	class MoveCommand final : public AnalogCommand
	{
	public:
		MoveCommand(GameObject* pGameObject, float moveSpeed);
		~MoveCommand() = default;

		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) = delete;
		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand& operator=(MoveCommand&& other) = delete;

		void Execute() override;

	private:
		const float m_MoveSpeed;
		RigidbodyComponent* m_pRigidBody{};

	};

	class GridMoveCommand final : public AnalogCommand
	{
	public:
		GridMoveCommand(GameObject* pGameObject, float moveSpeed);
		~GridMoveCommand() = default;

		GridMoveCommand(const GridMoveCommand& other) = delete;
		GridMoveCommand(GridMoveCommand&& other) = delete;
		GridMoveCommand& operator=(const GridMoveCommand& other) = delete;
		GridMoveCommand& operator=(GridMoveCommand&& other) = delete;

		void Execute() override;

	private:
		const float m_MoveSpeed;
		RigidbodyComponent* m_pRigidBody{};
	};

	class RotationCommand final : public AnalogCommand
	{
	public:
		RotationCommand(GameObject* pGameObject, float rotationSpeed);
		~RotationCommand() = default;

		RotationCommand(const RotationCommand& other) = delete;
		RotationCommand(RotationCommand&& other) = delete;
		RotationCommand& operator=(const RotationCommand& other) = delete;
		RotationCommand& operator=(RotationCommand&& other) = delete;

		void Execute() override;

	private:
		const float m_RotationSpeed;

		TransformComponent* m_pTransform{};
	};

	class LoadSceneCommand final : public Command
	{
	public:
		LoadSceneCommand(GameObject* pGameObject, const std::string sceneName);
		~LoadSceneCommand() = default;

		LoadSceneCommand(const LoadSceneCommand& other) = delete;
		LoadSceneCommand(LoadSceneCommand&& other) = delete;
		LoadSceneCommand& operator=(const LoadSceneCommand& other) = delete;
		LoadSceneCommand& operator=(LoadSceneCommand&& other) = delete;

		void Execute() override;

	private:
		const std::string m_SceneName;
	};

	class LoadNextSceneCommand final : public Command
	{
	public:
		LoadNextSceneCommand(GameObject* pGameObject);
		~LoadNextSceneCommand() = default;

		LoadNextSceneCommand(const LoadNextSceneCommand& other) = delete;
		LoadNextSceneCommand(LoadNextSceneCommand&& other) = delete;
		LoadNextSceneCommand& operator=(const LoadNextSceneCommand& other) = delete;
		LoadNextSceneCommand& operator=(LoadNextSceneCommand&& other) = delete;

		void Execute() override;
	};

	class LoadPreviousSceneCommand final : public Command
	{
	public:
		LoadPreviousSceneCommand(GameObject* pGameObject);
		~LoadPreviousSceneCommand() = default;

		LoadPreviousSceneCommand(const LoadPreviousSceneCommand& other) = delete;
		LoadPreviousSceneCommand(LoadPreviousSceneCommand&& other) = delete;
		LoadPreviousSceneCommand& operator=(const LoadPreviousSceneCommand& other) = delete;
		LoadPreviousSceneCommand& operator=(LoadPreviousSceneCommand&& other) = delete;

		void Execute() override;
	};

	class MoveInDirection final : public Command
	{
	public:
		MoveInDirection(GameObject* pGameObject, float moveSpeed, glm::vec2 direction);
		~MoveInDirection() = default;

		MoveInDirection(const MoveInDirection& other) = delete;
		MoveInDirection(MoveInDirection&& other) = delete;
		MoveInDirection& operator=(const MoveInDirection& other) = delete;
		MoveInDirection& operator=(MoveInDirection&& other) = delete;

		void Execute() override;

	private:
		const glm::vec2 m_Direction;
		const float m_MoveSpeed;
		
		RigidbodyComponent* m_pRigidBody{ nullptr };


	};

	class MuteCommand final : public Command
	{
	public:
		MuteCommand(GameObject* pGameObject);
		~MuteCommand() = default;

		MuteCommand(const MuteCommand& other) = delete;
		MuteCommand(MuteCommand&& other) = delete;
		MuteCommand& operator=(const MuteCommand& other) = delete;
		MuteCommand& operator=(MuteCommand&& other) = delete;

		void Execute() override;
	};

	class RotateDegreesCommand final : public Command
	{
	public:
		RotateDegreesCommand(GameObject* pGameObject, float Degrees);
		~RotateDegreesCommand() = default;

		RotateDegreesCommand(const RotateDegreesCommand& other) = delete;
		RotateDegreesCommand(RotateDegreesCommand&& other) = delete;
		RotateDegreesCommand& operator=(const RotateDegreesCommand& other) = delete;
		RotateDegreesCommand& operator=(RotateDegreesCommand&& other) = delete;

		void Execute() override;

	private:
		const float m_DegreesStep;

		TransformComponent* m_pTransform{ nullptr };
	};
	
	class TestCommand final : public Command
	{
	public:
		TestCommand(GameObject* pGameObject);
		~TestCommand() = default;

		TestCommand(const TestCommand& other) = delete;
		TestCommand(TestCommand&& other) = delete;
		TestCommand& operator=(const TestCommand& other) = delete;
		TestCommand& operator=(TestCommand&& other) = delete;

		void Execute() override;
	};

}

