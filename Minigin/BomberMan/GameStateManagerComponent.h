#pragma once
#include "Component.h"
#include <memory>

#include "GameState.h"
#include "Observer.h"

namespace dae
{
	class GameStateManagerComponent final : public Component
	{
	public:
		GameStateManagerComponent();
		~GameStateManagerComponent() = default;

		GameStateManagerComponent(const GameStateManagerComponent& other) = delete;
		GameStateManagerComponent(GameStateManagerComponent&& other) = delete;
		GameStateManagerComponent& operator=(const GameStateManagerComponent& other) = delete;
		GameStateManagerComponent& operator=(GameStateManagerComponent&& other) = delete;

		void Initialize(const std::string& currentScene, const std::string& nextScene, const std::string& startScene, const std::string& leaderbordScene);
		void Update() override;

	private:
		std::unique_ptr<GameState> m_CurrentGameState;

		std::string m_CurrentScene{};
		std::string m_NextScene{};
		std::string m_StartScene{};
		std::string m_LeaderbordScene{};
	};	
}

