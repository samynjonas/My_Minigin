#include "TextInputComponent.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "TextComponent.h"

#include "Scene.h"

#include <iostream>

dae::TextInputComponent::TextInputComponent()
	: Component()
{

}

void dae::TextInputComponent::Update()
{

}

void dae::TextInputComponent::CreateTextBox()
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto TextObject = std::make_shared<dae::GameObject>();

	auto scene = GetOwner()->GetScene();
	scene->Add(TextObject);

	TextObject->Initialize("TextObject", scene);
	TextObject->SetParent(GetOwner());

	auto textComp = TextObject->AddComponent<TextComponent>();
	textComp->Initialize("", font);
}