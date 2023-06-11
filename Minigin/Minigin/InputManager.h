#pragma once
#include "Singleton.h"
#include <vector>
#include <unordered_map>
#include <memory>

#include "SDL.h"
#include "Controller.h"
#include "Command.h"
#include "ActionCommands.h"


namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class InputType
		{
			OnButtonDown,
			OnButtonUp,
			OnButtonPress,
			OnAnalog
		};

		struct GamepadinputInfo
		{
		public:
			bool operator<(const GamepadinputInfo& other) const
			{ 
				return other.playerIndex < playerIndex; 
			}

			int playerIndex;
			unsigned int button{};
			InputType type{};
		};

		struct KeyboardInputInfo
		{
		public:
			bool operator<(const GamepadinputInfo& other) const
			{
				return other.playerIndex < playerIndex;
			}

			int playerIndex;
			SDL_Scancode button{};
			InputType type{};
		};


		bool ProcessInput();
		void BindCommand(const unsigned int& button, InputType inputType, std::unique_ptr<Command> pCommand, int playerIndex);
		void BindCommand(SDL_Scancode keyboardButton, InputType inputType, std::unique_ptr<Command> pCommand, int playerIndex);

	private:
		std::vector<std::unique_ptr<dae::Controller>> m_pControllers{};
		
		std::vector<GamepadinputInfo> m_ControllerInputInfo;
		std::vector<std::unique_ptr<Command>> m_ControllerCommands{};

		std::vector<KeyboardInputInfo> m_KeyboardInputInfo;
		std::vector<std::unique_ptr<Command>> m_KeyboardCommands{};

		void HandleControllerID(int playerIndex);
	};

}
