#include "LeaderbordComponent.h"

#include "ScoreSaver.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "Scene.h"

void dae::LeaderbordComponent::DisplayLeaderbord()
{
	ScoreSaver scoreParser{ "../Data/Highscores.json" };

	auto vecScores = scoreParser.GetHighScores();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	AddText({ 250.f, 50.f }, "HIGHSCORE", font);

	for (int i = 0; i < vecScores.size(); i++)
	{
		if (i < MAX_SCORES_DISPLAYED)
		{
			AddText({ 200.f, 125.f + 25.f * i }, { vecScores[i].name + " : " + std::to_string(vecScores[i].score)}, font);
		}
	}
}

void dae::LeaderbordComponent::AddText(glm::vec2 position, const std::string& text, std::shared_ptr<dae::Font> font)
{
	auto TextObject = std::make_shared<dae::GameObject>();

	auto scene = GetOwner()->GetScene();
	scene->Add(TextObject);

	TextObject->Initialize("TextObject", scene);
	TextObject->SetParent(GetOwner());

	auto textComp = TextObject->AddComponent<TextComponent>();
	textComp->Initialize(text, font);
	TextObject->transform()->SetLocalPosition(position);
}