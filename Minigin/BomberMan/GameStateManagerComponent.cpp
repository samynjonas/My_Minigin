#include "GameStateManagerComponent.h"


dae::GameStateManagerComponent::GameStateManagerComponent()
	: Component()
{

}

void dae::GameStateManagerComponent::Initialize(const std::string& currentScene, const std::string& nextScene, const std::string& startScene, const std::string& leaderbordScene)
{
	m_CurrentScene = currentScene;
	m_NextScene = nextScene;
	m_StartScene = startScene;
	m_LeaderbordScene = leaderbordScene;

	m_CurrentGameState = std::make_unique<Loading>();
	m_CurrentGameState->Initialize(currentScene, nextScene, startScene, leaderbordScene);
}

void dae::GameStateManagerComponent::Update()
{
	std::unique_ptr<GameState> newGameState = m_CurrentGameState->Update();
	if (newGameState != nullptr)
	{
		if (m_CurrentGameState)
		{
			m_CurrentGameState->Exit();
		}
		m_CurrentGameState = std::move(newGameState);

		m_CurrentGameState->Initialize(m_CurrentScene, m_NextScene, m_StartScene, m_LeaderbordScene);
		m_CurrentGameState->Enter();
	}
}