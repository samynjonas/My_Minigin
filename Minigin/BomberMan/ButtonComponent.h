#pragma once
#include "Component.h"
#include "Subject.h"

#include <functional>

namespace dae
{
	class ButtonComponent final : public Component, public Subject
	{
	public:
		ButtonComponent();
		~ButtonComponent() = default;

		ButtonComponent(const ButtonComponent& other) = delete;
		ButtonComponent(ButtonComponent&& other) = delete;
		ButtonComponent& operator=(const ButtonComponent& other) = delete;
		ButtonComponent& operator=(ButtonComponent&& other) = delete;
		
		void Update() override;

		void Select();
		void DeSelect();

		void Press();

	private:
		bool m_IsSelected{ false };

	};
}

