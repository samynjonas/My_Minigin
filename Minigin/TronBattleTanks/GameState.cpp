#include "GameState.h"
#include "SceneManager.h"

// --------------------------- Main Menu --------------------------------------

void dae::MainMenuState::Enter()
{
	SceneManager::GetInstance().LoadScene("MainMenu");
}

void dae::MainMenuState::Exit()
{

}

std::unique_ptr<dae::GameState> dae::MainMenuState::GameStateNotify(Event currEvent)
{
	if (currEvent == SinglePlayer)
	{
		return std::make_unique<Singleplayer_MAP1>();
	}
	else if (currEvent == Coop)
	{
		return std::make_unique<CoopState_MAP1>();
	}
	else if (currEvent == Versus)
	{
		return std::make_unique<VersusState>();
	}

	return nullptr;
}

// --------------------------- SinglePlayer --------------------------------------

void dae::SinglePlayerState::Enter()
{
	SceneManager::GetInstance().LoadScene("SceneMap1");
}

void dae::SinglePlayerState::Exit()
{

}

std::unique_ptr<dae::GameState> dae::SinglePlayerState::GameStateNotify(Event currEvent)
{
	if (currEvent == LiveLost)
	{
		//If player lost live
		//Reload current level
		SceneManager::GetInstance().LoadScene(SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (currEvent == ObjectDied)
	{
		//If player lost all lives
		//Change state to highscore state
		return std::make_unique<HighscoreMenuState>();
	}
	else if (currEvent == CounterFinished)
	{
		//If all enemies died
		//Load next map

	}
	return nullptr;
}


// --------------------------- SinglePlayer - MAP_1 --------------------------------------

void dae::Singleplayer_MAP1::Enter()
{
	SceneManager::GetInstance().LoadScene("SceneMap1");
}

void dae::Singleplayer_MAP1::Exit()
{

}

std::unique_ptr<dae::GameState> dae::Singleplayer_MAP1::GameStateNotify(Event currEvent)
{
	if (currEvent == LiveLost)
	{
		//If player lost live
		//Reload current level
		SceneManager::GetInstance().LoadScene(SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (currEvent == ObjectDied)
	{
		//If player lost all lives
		//Change state to highscore state
		return std::make_unique<HighscoreMenuState>();
	}
	else if (currEvent == CounterFinished)
	{
		//If all enemies died
		//Load next map
		return std::make_unique<Singleplayer_MAP2>();
	}
	return nullptr;
}

// --------------------------- SinglePlayer - MAP_2 --------------------------------------

void dae::Singleplayer_MAP2::Enter()
{
	SceneManager::GetInstance().LoadScene("SceneMap2");
}

void dae::Singleplayer_MAP2::Exit()
{

}

std::unique_ptr<dae::GameState> dae::Singleplayer_MAP2::GameStateNotify(Event currEvent)
{
	if (currEvent == LiveLost)
	{
		//If player lost live
		//Reload current level
		SceneManager::GetInstance().LoadScene(SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (currEvent == ObjectDied)
	{
		//If player lost all lives
		//Change state to highscore state
		return std::make_unique<HighscoreMenuState>();
	}
	else if (currEvent == CounterFinished)
	{
		//If all enemies died
		//Load next map
		return std::make_unique<Singleplayer_MAP3>();
	}
	return nullptr;
}

// --------------------------- SinglePlayer - MAP_3 --------------------------------------

void dae::Singleplayer_MAP3::Enter()
{
	SceneManager::GetInstance().LoadScene("SceneMap3");
}

void dae::Singleplayer_MAP3::Exit()
{

}

std::unique_ptr<dae::GameState> dae::Singleplayer_MAP3::GameStateNotify(Event currEvent)
{
	if (currEvent == LiveLost)
	{
		//If player lost live
		//Reload current level
		SceneManager::GetInstance().LoadScene(SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (currEvent == ObjectDied)
	{
		//If player lost all lives
		//Change state to highscore state
		return std::make_unique<HighscoreMenuState>();
	}
	else if (currEvent == CounterFinished)
	{
		//If all enemies died
		//Load next map
		return std::make_unique<Singleplayer_MAP1>();
	}
	return nullptr;
}


// --------------------------- COOP --------------------------------------

void dae::CoopState::Enter()
{
	SceneManager::GetInstance().LoadScene("SceneMap1");
}

void dae::CoopState::Exit()
{

}

std::unique_ptr<dae::GameState> dae::CoopState::GameStateNotify(Event currEvent)
{
	if (currEvent == LiveLost)
	{
		//If player lost live
		//Reload current level
		SceneManager::GetInstance().LoadScene(SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (currEvent == ObjectDied)
	{
		//If player lost all lives
		//Change state to highscore state
		return std::make_unique<HighscoreMenuState>();
	}
	else if (currEvent == CounterFinished)
	{
		//If all enemies died
		//Load next map

	}
	return nullptr;
}

// --------------------------- COOP - MAP_1 --------------------------------------

void dae::CoopState_MAP1::Enter()
{
	SceneManager::GetInstance().LoadScene("SceneMap1_Coop");
}

void dae::CoopState_MAP1::Exit()
{

}

std::unique_ptr<dae::GameState> dae::CoopState_MAP1::GameStateNotify(Event currEvent)
{
	if (currEvent == LiveLost)
	{
		//If player lost live
		//Reload current level
		SceneManager::GetInstance().LoadScene(SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (currEvent == ObjectDied)
	{
		//If player lost all lives
		//Change state to highscore state
		return std::make_unique<HighscoreMenuState>();
	}
	else if (currEvent == CounterFinished)
	{
		//If all enemies died
		//Load next map
		return std::make_unique<CoopState_MAP2>();
	}
	return nullptr;
}

// --------------------------- COOP - MAP_2 --------------------------------------

void dae::CoopState_MAP2::Enter()
{
	SceneManager::GetInstance().LoadScene("SceneMap2_Coop");
}

void dae::CoopState_MAP2::Exit()
{

}

std::unique_ptr<dae::GameState> dae::CoopState_MAP2::GameStateNotify(Event currEvent)
{
	if (currEvent == LiveLost)
	{
		//If player lost live
		//Reload current level
		SceneManager::GetInstance().LoadScene(SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (currEvent == ObjectDied)
	{
		//If player lost all lives
		//Change state to highscore state
		return std::make_unique<HighscoreMenuState>();
	}
	else if (currEvent == CounterFinished)
	{
		//If all enemies died
		//Load next map
		return std::make_unique<CoopState_MAP3>();
	}
	return nullptr;
}

// --------------------------- COOP - MAP_3 --------------------------------------

void dae::CoopState_MAP3::Enter()
{
	SceneManager::GetInstance().LoadScene("SceneMap3_Coop");
}

void dae::CoopState_MAP3::Exit()
{

}

std::unique_ptr<dae::GameState> dae::CoopState_MAP3::GameStateNotify(Event currEvent)
{
	if (currEvent == LiveLost)
	{
		//If player lost live
		//Reload current level
		SceneManager::GetInstance().LoadScene(SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (currEvent == ObjectDied)
	{
		//If player lost all lives
		//Change state to highscore state
		return std::make_unique<HighscoreMenuState>();
	}
	else if (currEvent == CounterFinished)
	{
		//If all enemies died
		//Load next map
		return std::make_unique<CoopState_MAP1>();
	}
	return nullptr;
}

// --------------------------- Versus --------------------------------------

void dae::VersusState::Enter()
{
	SceneManager::GetInstance().LoadScene("SceneMap1");
}

void dae::VersusState::Exit()
{

}

std::unique_ptr<dae::GameState> dae::VersusState::GameStateNotify(Event currEvent)
{
	if (currEvent == LiveLost)
	{
		//If player lost live
		//Reload current level
		SceneManager::GetInstance().LoadScene(SceneManager::GetInstance().GetActiveSceneName());
	}
	else if (currEvent == ObjectDied)
	{
		//If player lost all lives
		//Change state to highscore state
		return std::make_unique<HighscoreMenuState>();
	}
	else if (currEvent == CounterFinished)
	{
		//If all enemies died
		//Load next map

	}
	return nullptr;
}

// --------------------------- HighScore Menu --------------------------------------
void dae::HighscoreMenuState::Enter()
{
	SceneManager::GetInstance().LoadScene("HighScoreMenu");
}

void dae::HighscoreMenuState::Exit()
{

}

std::unique_ptr<dae::GameState> dae::HighscoreMenuState::GameStateNotify(Event)
{
	return nullptr;
}