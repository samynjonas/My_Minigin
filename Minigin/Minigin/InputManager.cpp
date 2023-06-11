// https://stackoverflow.com/questions/11040133/what-does-defining-win32-lean-and-mean-exclude-exactly
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl.h>

#include <iostream>

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) 
		{
			return false;
		}

		if (e.type == SDL_KEYDOWN) 
		{
			for (size_t index = 0; index < m_KeyboardInputInfo.size(); index++)
			{
				if (m_KeyboardInputInfo[index].type == InputType::OnButtonDown)
				{
					if (e.key.keysym.scancode == m_KeyboardInputInfo[index].button)
					{
						m_KeyboardCommands[index].get()->Execute();
					}
				}
			}
		}
		if (e.type == SDL_KEYUP) 
		{
			for (size_t index = 0; index < m_KeyboardInputInfo.size(); index++)
			{
				if (m_KeyboardInputInfo[index].type == InputType::OnButtonUp)
				{
					if (e.key.keysym.scancode == m_KeyboardInputInfo[index].button)
					{
						m_KeyboardCommands[index].get()->Execute();
					}
				}
			}
		}
	}

	for (const auto& controller : m_pControllers)
	{
		controller->Update();
	}

	bool isActivated{ false };

	for (size_t index = 0; index < m_ControllerInputInfo.size(); index++)
	{
		Command* command = m_ControllerCommands[index].get();
		switch (m_ControllerInputInfo[index].type)
		{
		case InputType::OnButtonDown:
			isActivated = m_pControllers[m_ControllerInputInfo[index].playerIndex]->IsDown(m_ControllerInputInfo[index].button);
			break;
		case InputType::OnButtonUp:
			isActivated = m_pControllers[m_ControllerInputInfo[index].playerIndex]->isUp(m_ControllerInputInfo[index].button);
			break;
		case InputType::OnButtonPress:
			isActivated = m_pControllers[m_ControllerInputInfo[index].playerIndex]->isPressed(m_ControllerInputInfo[index].button);
			break;
		case InputType::OnAnalog:
		{
			//TODO improve this
			MoveCommand* moveCommand = dynamic_cast<MoveCommand*>(command);
			if (moveCommand)
			{
				moveCommand->SetAxisValue(m_pControllers[m_ControllerInputInfo[index].playerIndex]->GetAxis(true));
				isActivated = true;
			}
			else
			{
				GridMoveCommand* gridMovement = dynamic_cast<GridMoveCommand*>(command);
				if (gridMovement)
				{
					gridMovement->SetAxisValue(m_pControllers[m_ControllerInputInfo[index].playerIndex]->GetAxis(true));
					isActivated = true;
				}
				else
				{
					RotationCommand* rotation = dynamic_cast<RotationCommand*>(command);
					if (rotation)
					{
						rotation->SetAxisValue(m_pControllers[m_ControllerInputInfo[index].playerIndex]->GetAxis(false));
						isActivated = true;
					}
				}
			}
		}
		break;
		default:
			break;
		}

		if (isActivated)
		{
			command->Execute();
		}
	}
	return true;
}


void dae::InputManager::BindCommand(const unsigned int& button, InputType inputType, std::unique_ptr<Command> pCommand, int playerIndex)
{
	if (playerIndex >= static_cast<int>(m_pControllers.size()))
	{
		HandleControllerID(playerIndex);
	}

	GamepadinputInfo info{};
	info.button			= button;
	info.type			= inputType;
	info.playerIndex	= playerIndex;

	m_ControllerInputInfo.push_back(info);
	m_ControllerCommands.push_back(std::move(pCommand));
}

void dae::InputManager::BindCommand(SDL_Scancode keyboardButton, InputType inputType, std::unique_ptr<Command> pCommand, int playerIndex)
{
	if (playerIndex >= static_cast<int>(m_pControllers.size()))
	{
		HandleControllerID(playerIndex);
	}

	KeyboardInputInfo info{};
	info.button = keyboardButton;
	info.type = inputType;
	info.playerIndex = playerIndex;

	m_KeyboardInputInfo.push_back(info);
	m_KeyboardCommands.push_back(std::move(pCommand));
}

void dae::InputManager::HandleControllerID(int playerIndex)
{
	for (int index = static_cast<int>(m_pControllers.size()) - 1; index < playerIndex + 1; ++index)
	{
		std::cout << "Controller: " << playerIndex << "connected" << std::endl;

		//Add new controller to vector
		m_pControllers.push_back(std::make_unique<Controller>(static_cast<int>(m_pControllers.size())));
	}
}