#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <memory>
#include <iostream>
#include <string>

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"

#include "GameObject.h"
#include "Controller.h"
#include "InputManager.h"
#include "ActionCommands.h"
#include "Commands.h"

#include "Achievements.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "GunComponent.h"
#include "MapGeneratorComponent.h"
#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"
#include "AI_BehaviourComponent.h"

#include "servicelocator.h"
#include "sound_system.h"
#include "sdl_sound_system.h"
#include "Loggin_Sound_System.h"

#include "Logger.h"
#include "Console_Logger.h"

using namespace dae;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

#if _DEBUG
	servicelocator<Logger>::register_service_locator(std::make_unique<Console_Logger>());
	servicelocator<sound_system>::register_service_locator(std::make_unique<Loggin_Sound_System>(std::make_unique<sdl_sound_system>()));

#else
	servicelocator<Logger>::register_service_locator(new Console_Logger()); //TODO change to file logger
	servicelocator<sound_system>::register_service_locator(new sdl_sound_system);
#endif

	servicelocator<sound_system>::get_serviceLocator().AddSound("Shoot", 64, "../Data/Sounds/Shoot.wav");

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	std::unique_ptr<Achievements> pAchiementObserver = std::make_unique<Achievements>();

	auto background = std::make_shared<dae::GameObject>();
	{
		background->Initialize("background_GB", &scene);
		background->renderer()->SetTexture("background.tga");
		scene.Add(background);
	}

	auto remainingLivesGO = std::make_shared<dae::GameObject>();
	remainingLivesGO->Initialize("RemainingLives", &scene);
	TextComponent* pRemainingLives = remainingLivesGO->AddComponent<dae::TextComponent>();
	pRemainingLives->Initialize("Remaining lives = 1", font);
	remainingLivesGO->transform()->SetLocalPosition({ 25, 350 });
	scene.Add(remainingLivesGO);

	auto scoreGO = std::make_shared<dae::GameObject>();
	scoreGO->Initialize("RemainingLives", &scene);
	TextComponent* pScoreTXT = scoreGO->AddComponent<dae::TextComponent>();
	pScoreTXT->Initialize("Remaining lives = 1", font);
	scoreGO->transform()->SetLocalPosition({ 25, 400 });
	scene.Add(scoreGO);

	auto map = std::make_shared<dae::GameObject>();
	{
		map->Initialize("Map", &scene);
		scene.Add(map);	
		
		map->transform()->SetLocalPosition({50.f, 0.f});

		auto mapComponent = map->AddComponent<dae::MapGeneratorComponent>();
		mapComponent->Initialize("../Data/Level/LevelLayout1.csv", 16);
	}

	auto TitleGo = std::make_shared<GameObject>();
	{
		TitleGo->Initialize("Text_GB", &scene);

		auto textcomp = TitleGo->AddComponent<TextComponent>();

		textcomp->Initialize("Tron battle tanks", font);

		TitleGo->transform()->SetLocalPosition({ 80, 20 });
		scene.Add(TitleGo);
	}
}


int main(int, char* []) 
{
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}