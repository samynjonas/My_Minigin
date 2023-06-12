#pragma once
#include "Component.h"

namespace dae
{
	class TextComponent;
	class TextInputComponent final : public Component
	{
	public:
		TextInputComponent();
		~TextInputComponent() = default;

		TextInputComponent(const TextInputComponent& other) = delete;
		TextInputComponent(TextInputComponent&& other) = delete;
		TextInputComponent& operator=(const TextInputComponent& other) = delete;
		TextInputComponent& operator=(TextInputComponent&& other) = delete;

		void Update() override;

	private:

		void CreateTextBox();
		
		TextComponent* m_TextComponent{ nullptr };

	};
}


