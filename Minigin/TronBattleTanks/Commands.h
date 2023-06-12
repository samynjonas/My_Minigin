#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;
	class TransformComponent;
	class GunComponent;

	class ShootCommand final : public Command
	{
	public:
		ShootCommand(GameObject* pGameObject);
		~ShootCommand() = default;

		ShootCommand(const ShootCommand& other) = delete;
		ShootCommand(ShootCommand&& other) = delete;
		ShootCommand& operator=(const ShootCommand& other) = delete;
		ShootCommand& operator=(ShootCommand&& other) = delete;

		void Execute() override;

	private:
		GunComponent* m_pGunComponent{ nullptr };

	};

	class TestCommand_playSound final : public Command
	{
	public:
		TestCommand_playSound(GameObject* pGameObject);
		~TestCommand_playSound() = default;

		TestCommand_playSound(const TestCommand_playSound& other) = delete;
		TestCommand_playSound(TestCommand_playSound&& other) = delete;
		TestCommand_playSound& operator=(const TestCommand_playSound& other) = delete;
		TestCommand_playSound& operator=(TestCommand_playSound&& other) = delete;

		void Execute() override;
	};

	class LetterUpCommand final : public Command
	{
		LetterUpCommand(GameObject* pGameObject);
		~LetterUpCommand() = default;

		LetterUpCommand(const LetterUpCommand& other) = delete;
		LetterUpCommand(LetterUpCommand&& other) = delete;
		LetterUpCommand& operator=(const LetterUpCommand& other) = delete;
		LetterUpCommand& operator=(LetterUpCommand&& other) = delete;

		void Execute() override;

	private:

	};

	class LetterDownCommand final : public Command
	{
		LetterDownCommand(GameObject* pGameObject);
		~LetterDownCommand() = default;

		LetterDownCommand(const LetterDownCommand& other) = delete;
		LetterDownCommand(LetterDownCommand&& other) = delete;
		LetterDownCommand& operator=(const LetterDownCommand& other) = delete;
		LetterDownCommand& operator=(LetterDownCommand&& other) = delete;

		void Execute() override;

	private:

	};

}

