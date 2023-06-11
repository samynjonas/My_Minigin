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

}

