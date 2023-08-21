#include "GameState.h"
#include "MiniginTimer.h"

#include "Scene.h"
#include "SceneManager.h"
#include "GameListener.h"


void dae::GameState::Initialize(const std::string& CurrentScene, const std::string& NextScene, const std::string& StartScene, const std::string& LeaderbordScene)
{
	m_CurrentScene = CurrentScene;
	m_NextScene = NextScene;
	m_StartScene = StartScene;
	m_LeaderbordScene = LeaderbordScene;
}

std::unique_ptr<dae::GameState> dae::Loading::Update()
{
	m_ElapsedTime += MiniginTimer::GetInstance().GetDeltaTime();

	if (m_ElapsedTime >= LOAD_TIME)
	{
		//LOAD GAME
		//SceneManager::GetInstance().LoadScene(m_CurrentScene);
		return std::make_unique<InGame>();
	}

	return nullptr;
}


void dae::Loading::Enter()
{
	m_ElapsedTime = 0;
}

void dae::Loading::Exit()
{

}


std::unique_ptr<dae::GameState> dae::InGame::Update()
{
	auto& GameListener = GameListener::GetInstance();

	if (GameListener.GetHasPlayerDied())
	{
		GameListener.SetHasPlayerDied(false);
		if (GameListener.GetHasLivesLeft())
		{
			GameListener.SetHasLivesLeft(true);
			return std::make_unique<Loading>();
		}
		else
		{
			//Load Leaderbord scene
			SceneManager::GetInstance().LoadScene(m_LeaderbordScene);
		}
	}
	else if (GameListener.GetHasSuccesFullyOpenedDoor())
	{
		GameListener.SetHasSuccesFullyOpenedDoor(false);
		if (m_NextScene == "")
		{
			//Was Last scene
			SceneManager::GetInstance().LoadScene(m_LeaderbordScene);
		}
		else
		{
			SceneManager::GetInstance().LoadScene(m_NextScene);
		}
	}
	else if (GameListener.GetHasPausedGame())
	{
		GameListener.SetHasPausedGame(false);
		return std::make_unique<Paused>();
	}
	return nullptr;
}

void dae::InGame::Enter()
{

}

void dae::InGame::Exit()
{

}

std::unique_ptr<dae::GameState> dae::Paused::Update()
{
	bool Continued{ true };
	if (Continued)
	{
		return std::make_unique<InGame>();
	}
	return nullptr;
}

void dae::Paused::Enter()
{

}

void dae::Paused::Exit()
{

}