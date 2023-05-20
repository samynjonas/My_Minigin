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

	for (const auto& command : m_ControllerCommands)
	{
		switch (command.first.type)
		{
		case InputType::OnButtonDown:
			isActivated = m_pControllers[command.first.playerIndex]->IsDown(command.first.buttons[0]);
			break;
		case InputType::OnButtonUp:
			isActivated = m_pControllers[command.first.playerIndex]->isUp(command.first.buttons[0]);
			break;
		case InputType::OnButtonPress:
			isActivated = m_pControllers[command.first.playerIndex]->isPressed(command.first.buttons[0]);
			break;
		default:
			break;
		}

		if (isActivated)
		{
			command.second->Execute();
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

	m_ControllerCommands.emplace(info, std::move(pCommand));
}

void dae::InputManager::HandleControllerID(int playerIndex)
{
	for (int index = static_cast<int>(m_pControllers.size()) - 1; index < playerIndex + 1; ++index)
	{
		std::cout << "Controller connected" << std::endl;

		//Add new controller to vector
		m_pControllers.push_back(std::make_unique<Controller>(static_cast<int>(m_pControllers.size())));
	}
}