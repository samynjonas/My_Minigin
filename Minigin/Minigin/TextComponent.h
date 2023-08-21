#pragma once
#include "Component.h"
#include <string>
#include <memory>

#include <SDL_ttf.h>
#include <functional>

#include "Observer.h"

namespace dae
{
	class Font;
	class RenderComponent;
	class TextComponent final : public Component, public Observer
	{
	public:
		TextComponent();
		~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void Update() override;

		void SetText(const std::string& text);

		void SetGetterFunction(std::function<int()> getterFunction);
		void SetGetterFunction(std::function<std::string()> getterFunction);


		void SetFont(std::shared_ptr<Font> font);
		void Initialize(const std::string& text, std::shared_ptr<Font> font, SDL_Color fontColor = { 255, 255, 255 }, short renderDepth = 1);

		virtual void Notify(Event currEvent, Subject* actor) override;

	private:
		std::string m_Text{};
		std::shared_ptr<Font> m_Font{ nullptr };
		SDL_Color m_TextColor{};
		short m_RenderDepth{};

		RenderComponent* m_pRenderer{ nullptr };

		std::function<int()> m_GetterFuntion_Int{ nullptr };
		int m_PrevIntValue{ 0 };

		std::function<std::string()> m_GetterFuntion_String{ nullptr };

		void RetrieveNewValue();

		bool m_NeedsUpdate;
	};
}


