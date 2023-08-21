#include "Bomberman_Commands.h"

#include "GameObject.h"

#include "BombermanBehaviourComponent.h"
#include "ButtonComponent.h"
#include "GameModePickerComponent.h"

dae::PlaceBombCommand::PlaceBombCommand(GameObject* pGameObject)
	: Command(pGameObject)
{
	m_pBombermanBehaviour = pGameObject->GetComponent<BombermanBehaviourComponent>();
}

void dae::PlaceBombCommand::Execute()
{
	if (m_pBombermanBehaviour != nullptr)
	{
		m_pBombermanBehaviour->PlaceBomb();
	}
}

dae::DetonateBomb::DetonateBomb(GameObject* pGameObject)
	: Command(pGameObject)
{
	m_pBombermanBehaviour = pGameObject->GetComponent<BombermanBehaviourComponent>();
}

void dae::DetonateBomb::Execute()
{
	if (m_pBombermanBehaviour != nullptr)
	{
		m_pBombermanBehaviour->Detonate();
	}
}

dae::PressButton::PressButton(GameObject* pGameObject)
	: Command(pGameObject)
{
	m_pButton = pGameObject->GetComponent<ButtonComponent>();
}

void dae::PressButton::Execute()
{
	if (m_pButton)
	{
		m_pButton->Press();
	}
}

dae::CyclePicker::CyclePicker(GameObject* pGameObject)
	: Command(pGameObject)
{
	m_pPicker = pGameObject->GetComponent<GameModePickerComponent>();
}

void dae::CyclePicker::Execute()
{
	if (m_pPicker)
	{
		m_pPicker->Next();
	}
}