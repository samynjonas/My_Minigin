#pragma once
#include "Component.h"
#include <string>
#include <memory>

#include <SDL_ttf.h>

#include "Observer.h"

namespace dae
{
	class Font;
	class RenderComponent;
	class TextComponent final : public Component, public Observer
	{
	public:
		TextComponent();
		~TextComponent();
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void Update() override;

		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<Font> font);
		void Initialize(const std::string& text, std::shared_ptr<Font> font, SDL_Color fontColor = { 255, 255, 255 });

		virtual void Notify(Event currEvent, subject* actor) override;

	private:
		std::string m_Text{};
		std::shared_ptr<Font> m_Font{ nullptr };
		SDL_Color m_TextColor{};

		bool m_NeedsUpdate;
	};
}


