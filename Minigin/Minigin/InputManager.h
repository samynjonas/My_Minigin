#pragma once
#include "Singleton.h"
#include <vector>
#include <unordered_map>
#include <memory>

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

		struct inputInfo
		{
		public:
			bool operator<(const inputInfo& other) const { return other.playerIndex < playerIndex; }

			int playerIndex;
			std::vector<unsigned int> buttons{};
			InputType type{};
		};


		bool ProcessInput();
		void BindCommand(const std::vector<unsigned int>& buttons, InputType inputType, std::unique_ptr<Command> pCommand, int playerIndex);

	private:
		std::vector<std::unique_ptr<dae::Controller>> m_pControllers{};
		
		std::vector<inputInfo> m_InputInfo;
		std::vector<std::unique_ptr<Command>> m_ControllerCommands{};

		void HandleControllerID(int playerIndex);
	};

}
