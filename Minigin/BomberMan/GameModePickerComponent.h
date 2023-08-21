#pragma once
#include "Component.h"

#include <memory>
#include <vector>
#include <string>

#include "Observer.h"

namespace dae
{
	class GameModes;
	class GameModePickerComponent final : public Component, public Observer
	{
	public:
		GameModePickerComponent();
		~GameModePickerComponent() = default;

		GameModePickerComponent(const GameModePickerComponent& other) = delete;
		GameModePickerComponent(GameModePickerComponent&& other) = delete;
		GameModePickerComponent& operator=(const GameModePickerComponent& other) = delete;
		GameModePickerComponent& operator=(GameModePickerComponent&& other) = delete;

		void Initialize();
		void AddGameMode(std::unique_ptr<dae::GameModes> pGameMode, std::string gameModeName);
		void Update() override;

		void Next();
		void Previous();

		void Notify(Event CurrEvent, Subject* subject) override;

		std::unique_ptr<GameModes> GetGameMode();	
		std::string GetGameModeNames() const;

	private:
		std::vector<std::unique_ptr<dae::GameModes>> m_VecGameModes;
		std::vector<std::string> m_VecGameModeNames;

		int m_CurrentGameMode{ 0 };

	};
}

