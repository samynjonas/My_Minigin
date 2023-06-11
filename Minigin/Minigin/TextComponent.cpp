#include "TextComponent.h"
#include <stdexcept>
#include "Font.h"

#include "Renderer.h"

#include "GameObject.h"
#include "Texture2D.h"

#include "RenderComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

dae::TextComponent::TextComponent()
	: Component()
	, m_NeedsUpdate{ false }
{

}

dae::TextComponent::~TextComponent()
{

}

void dae::TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_Font = std::move(font);
	m_NeedsUpdate = true;
}

void dae::TextComponent::Initialize(const std::string& text, std::shared_ptr<Font> font, SDL_Color fontColor)
{
	SetText(text);
	SetFont(font);

	m_TextColor = fontColor;
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

		GetOwner()->renderer()->SetTexture(std::make_shared<Texture2D>(texture));

		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Notify(Event currEvent, subject* pSubject)
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