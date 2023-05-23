#pragma once
#include "glm/glm.hpp"

namespace dae
{
	class Controller final
	{
	public:
		enum GamepadInput
		{
			DPAD_UP			= 0x0001,
			DPAD_DOWN		= 0x0002,
			DPAD_LEFT		= 0x0004,
			DPAD_RIGHT		= 0x0008,
			START			= 0x0010,
			BACK			= 0x0020,
			LEFT_THUMB		= 0x0040,
			RIGHT_THUMB		= 0x0080,
			LEFT_SHOULDER	= 0x0100,
			RIGHT_SHOULDER	= 0x0200,
			A				= 0x1000,
			B				= 0x2000,
			X				= 0x4000,
			Y				= 0x8000
		};

		explicit Controller(int controllerIndex);
		~Controller();

		void Update();

		bool IsDown(unsigned int button) const;
		bool isUp(unsigned int button) const;
		bool isPressed(unsigned int button) const;
		glm::vec2 GetAxis(bool isLeft) const;

		Controller(const Controller& other) = delete;
		Controller(Controller&& other) = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) = delete;

	private:
		class ControllerÎmpl;
		ControllerÎmpl* pImpl;

	};

}


