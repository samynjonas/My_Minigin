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
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
	}

	for (const auto& controller : m_pControllers)
	{
		controller->Update();
	}

	bool isActivated{ false };

	for (size_t index = 0; index < m_InputInfo.size(); index++)
	{
		Command* command = m_ControllerCommands[index].get();
		switch (m_InputInfo[index].type)
		{
		case InputType::OnButtonDown:
			isActivated = m_pControllers[m_InputInfo[index].playerIndex]->IsDown(m_InputInfo[index].buttons[0]);
			break;
		case InputType::OnButtonUp:
			isActivated = m_pControllers[m_InputInfo[index].playerIndex]->isUp(m_InputInfo[index].buttons[0]);
			break;
		case InputType::OnButtonPress:
			isActivated = m_pControllers[m_InputInfo[index].playerIndex]->isPressed(m_InputInfo[index].buttons[0]);
			break;
		case InputType::OnAnalog:
		{
			//TODO improve this
			MoveCommand* moveCommand = dynamic_cast<MoveCommand*>(command);
			if (moveCommand)
			{
				moveCommand->SetAxisValue(m_pControllers[m_InputInfo[index].playerIndex]->GetAxis(true));
				isActivated = true;
			}
			else
			{
				GridMoveCommand* gridMovement = dynamic_cast<GridMoveCommand*>(command);
				if (gridMovement)
				{
					gridMovement->SetAxisValue(m_pControllers[m_InputInfo[index].playerIndex]->GetAxis(true));
					isActivated = true;
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


void dae::InputManager::BindCommand(const std::vector<unsigned int>& buttons, InputType inputType, std::unique_ptr<Command> pCommand, int playerIndex)
{
	if (buttons.empty()) //If there are no keys given
	{
		return;
	}

	if (playerIndex >= static_cast<int>(m_pControllers.size()))
	{
		HandleControllerID(playerIndex);
	}

	inputInfo info{};
	info.buttons = buttons;
	info.type = inputType;
	info.playerIndex = playerIndex;

	m_InputInfo.push_back(info);
	m_ControllerCommands.push_back(std::move(pCommand));
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