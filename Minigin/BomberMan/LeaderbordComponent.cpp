#include "LeaderbordComponent.h"

#include "ScoreConverter.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "Scene.h"

void dae::LeaderbordComponent::DisplayLeaderbord()
{
	ScoreConverter scoreParser{ "../Data/Highscores.json" };

	auto vecScores = scoreParser.GetHighScores();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	AddText({ 250.f, 50.f }, "HIGHSCORE", font);

	for (int i = 0; i < static_cast<int>(vecScores.size()); i++)
	{
		if (i < MAX_SCORES_DISPLAYED)
		{
			AddText({ 200.f, 125.f + 25.f * i }, { vecScores[i].name + " : " + std::to_string(vecScores[i].score) }, font);
		}
	}
}

void dae::LeaderbordComponent::AddText(glm::vec2 position, const std::string& text, std::shared_ptr<dae::Font> font)
{
	auto textObject = std::make_unique<dae::GameObject>();
	auto pTextObject = textObject.get();
	auto pScene = GetOwner()->GetScene();

	pScene->Add(std::move(textObject));

	pTextObject->Initialize("TextObject", pScene);
	pTextObject->SetParent(GetOwner());

	auto textComp = pTextObject->AddComponent<TextComponent>();
	textComp->Initialize(text, font);
	pTextObject->transform()->SetLocalPosition(position);
}