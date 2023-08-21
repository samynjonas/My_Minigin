#include "TextComponent.h"
#include <stdexcept>
#include "Font.h"

#include "Renderer.h"

#include "GameObject.h"
#include "Texture2D.h"

#include "RenderComponent.h"
#include "../TronBattleTanks/ScoreComponent.h"
#include "../TronBattleTanks/HealthComponent.h"

dae::TextComponent::TextComponent()
	: Component()
	, m_NeedsUpdate{ false }
{

}

void dae::TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_Font = std::move(font);
	m_NeedsUpdate = true;
}

void dae::TextComponent::Initialize(const std::string& text, std::shared_ptr<Font> font, SDL_Color fontColor, short renderDepth)
{
	SetText(text);
	SetFont(font);

	m_TextColor = fontColor;
	m_RenderDepth = renderDepth;

	m_pRenderer = GetOwner()->GetComponent<RenderComponent>();
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = GetOwner()->AddComponent<RenderComponent>();
	}
	m_pRenderer->Initialize(renderDepth, true);
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = m_TextColor;
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		const auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);

		if (m_pRenderer)
		{
			m_pRenderer->SetTexture(std::make_shared<Texture2D>(texture));
		}

		m_NeedsUpdate = false;
	}
	else
	{
		RetrieveNewValue();
	}
}


void dae::TextComponent::SetGetterFunction(std::function<int()> getterFunction)
{
	m_GetterFuntion_Int = getterFunction;
}

void dae::TextComponent::SetGetterFunction(std::function<std::string()> getterFunction)
{
	m_GetterFuntion_String = getterFunction;
}

void dae::TextComponent::Notify(Event currEvent, Subject* pSubject)
{
	if (currEvent == LiveLost)
	{
		HealthComponent* health = static_cast<HealthComponent*>(pSubject); //TODO improve this
		if (typeid(*health) == typeid(HealthComponent))
		{
			const std::string text = "Lives: ";

			SetText(text + std::to_string(health->GetLives()));
		}
	}
	else if (currEvent == ScoreUpdated)
	{
		ScoreComponent* score = static_cast<ScoreComponent*>(pSubject); //TODO improve this
		if (typeid(*score) == typeid(ScoreComponent))
		{
			const std::string text = "Score: ";

			SetText(text + std::to_string(score->GetScore()));
		}
	}
}

void dae::TextComponent::RetrieveNewValue()
{
	if (m_GetterFuntion_Int)
	{
		int newValue = m_GetterFuntion_Int();
		if (newValue == m_PrevIntValue)
		{
			return;
		}

		m_NeedsUpdate = true;
		m_Text = std::to_string(newValue);
		m_PrevIntValue = newValue;
	}
	else if (m_GetterFuntion_String)
	{
		std::string newValue = m_GetterFuntion_String();
		if (newValue == m_Text)
		{
			return;
		}

		m_NeedsUpdate = true;
		m_Text = newValue;
	}
}