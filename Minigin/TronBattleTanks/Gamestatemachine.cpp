#include "Gamestatemachine.h"
#include "GameState.h"

dae::Gamestatemachine::Gamestatemachine()
{
	
}

void dae::Gamestatemachine::Update()
{

}

void dae::Gamestatemachine::Start()
{
	m_State = std::make_unique<MainMenuState>();
	
	m_State->Enter();
}

void dae::Gamestatemachine::Notify(Event event, Subject*)
{
	std::unique_ptr<GameState> state = m_State->GameStateNotify(event);
	if (state != nullptr)
	{
		m_State->Exit();

		m_State = std::move(state);

		m_State->Enter();
	}
}