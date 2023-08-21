#include "GameModePickerComponent.h"
#include "GameModes.h"

dae::GameModePickerComponent::GameModePickerComponent()
	: Component()
{

}

void dae::GameModePickerComponent::Initialize()
{

}

void dae::GameModePickerComponent::AddGameMode(std::unique_ptr<dae::GameModes> pGameMode, std::string gameModeName)
{
	m_VecGameModes.push_back(std::move(pGameMode));
	m_VecGameModeNames.push_back(gameModeName);
}

void dae::GameModePickerComponent::Update()
{

}

void dae::GameModePickerComponent::Next()
{
	m_CurrentGameMode++;
	m_CurrentGameMode %= m_VecGameModes.size();
}

void dae::GameModePickerComponent::Previous()
{
	m_CurrentGameMode--;
	if (m_CurrentGameMode < 0)
	{
		m_CurrentGameMode = static_cast<int>(m_VecGameModes.size()) - 1;
	}
}

std::unique_ptr<dae::GameModes> dae::GameModePickerComponent::GetGameMode()
{
	if (m_CurrentGameMode < 0)
	{
		return nullptr;
	}

	if (m_CurrentGameMode < static_cast<int>(m_VecGameModes.size()))
	{
		return std::move(m_VecGameModes[m_CurrentGameMode]);
	}
	return nullptr;
}

std::string dae::GameModePickerComponent::GetGameModeNames() const
{
	if (m_CurrentGameMode < 0)
	{
		return "";
	}

	if (m_CurrentGameMode < static_cast<int>(m_VecGameModeNames.size()))
	{
		return m_VecGameModeNames[m_CurrentGameMode];
	}
	return "";
}

void dae::GameModePickerComponent::Notify(Event CurrEvent, Subject*)
{
	if (CurrEvent == ButtonPressed)
	{
		auto gameMode = GetGameMode();
		if (gameMode)
		{
			gameMode->InitializeScene();
		}
	}
}