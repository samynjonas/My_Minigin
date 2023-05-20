#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

#include "Controller.h"
#include <algorithm>

#include <iostream>

using namespace dae;

class Controller::ControllerÎmpl
{
public:
	ControllerÎmpl(int controllerIndex)
	{
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

		m_ControllerIndex = controllerIndex;
	}

	void Update()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame	= buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame	= buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}

	bool IsDown(unsigned int button) const
	{
		return m_ButtonsPressedThisFrame & button;
	}

	bool IsUp(unsigned int button) const
	{
		return m_ButtonsReleasedThisFrame & button;
	}

	bool IsPressed(unsigned int button) const
	{
		return m_CurrentState.Gamepad.wButtons & button;
	}

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonsPressedThisFrame;
	WORD m_ButtonsReleasedThisFrame;

	unsigned int m_ControllerIndex;

	const float MAX_AXIS_VALUE_STICKS{ 3276.f };
	const float MAX_TRIGGER_AXIS_VALUE{ 255.f };

};

Controller::Controller(int controllerIndex)
{
	pImpl = new ControllerÎmpl(controllerIndex);
}

Controller::~Controller()
{
	delete pImpl;
}

void Controller::Update()
{
	pImpl->Update();
}

bool Controller::IsDown(unsigned int button) const
{
	return pImpl->IsDown(button);
}

bool Controller::isUp(unsigned int button) const
{
	return pImpl->IsUp(button);
}

bool Controller::isPressed(unsigned int button) const
{
	return pImpl->IsPressed(button);
}