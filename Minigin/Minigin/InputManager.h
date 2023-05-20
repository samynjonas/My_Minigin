#pragma once
#include "Singleton.h"
#include <vector>
#include <map>
#include <memory>

#include "Controller.h"
#include "Command.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class InputType
		{
			OnButtonDown,
			OnButtonUp,
			OnButtonPress
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
		std::map<inputInfo, std::unique_ptr<Command>>m_ControllerCommands{};

		void HandleControllerID(int playerIndex);
	};

}
