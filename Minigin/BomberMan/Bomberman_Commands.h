#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;
	class BombermanBehaviourComponent;
	class ButtonComponent;
	class GameModePickerComponent;

	class PlaceBombCommand final : public Command
	{
	public:
		PlaceBombCommand(GameObject* pGameObject);
		~PlaceBombCommand() = default;

		PlaceBombCommand(const PlaceBombCommand& other) = delete;
		PlaceBombCommand(PlaceBombCommand&& other) = delete;
		PlaceBombCommand& operator=(const PlaceBombCommand& other) = delete;
		PlaceBombCommand& operator=(PlaceBombCommand&& other) = delete;

		void Execute() override;
	private:
		BombermanBehaviourComponent* m_pBombermanBehaviour{ nullptr };

	};


	class DetonateBomb final : public Command
	{
	public:
		DetonateBomb(GameObject* pGameObject);
		~DetonateBomb() = default;

		DetonateBomb(const DetonateBomb& other) = delete;
		DetonateBomb(DetonateBomb&& other) = delete;
		DetonateBomb& operator=(const DetonateBomb& other) = delete;
		DetonateBomb& operator=(DetonateBomb&& other) = delete;

		void Execute() override;
	private:
		BombermanBehaviourComponent* m_pBombermanBehaviour{ nullptr };

	};

	class PressButton final : public Command
	{
	public:
		PressButton(GameObject* pGameObject);
		~PressButton() = default;

		PressButton(const PressButton& other) = delete;
		PressButton(PressButton&& other) = delete;
		PressButton& operator=(const PressButton& other) = delete;
		PressButton& operator=(PressButton&& other) = delete;

		void Execute() override;
	private:
		ButtonComponent* m_pButton;

	};

	class CyclePicker final : public Command
	{
	public:
		CyclePicker(GameObject* pGameObject);
		~CyclePicker() = default;

		CyclePicker(const CyclePicker& other) = delete;
		CyclePicker(CyclePicker&& other) = delete;
		CyclePicker& operator=(const CyclePicker& other) = delete;
		CyclePicker& operator=(CyclePicker&& other) = delete;

		void Execute() override;
	private:
		GameModePickerComponent* m_pPicker;

	};
}