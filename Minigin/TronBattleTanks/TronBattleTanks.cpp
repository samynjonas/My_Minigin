#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <memory>
#include <iostream>

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
	servicelocator<Logger>::register_service_locator(new Console_Logger());
	servicelocator<sound_system>::register_service_locator(new Loggin_Sound_System(std::make_unique<sdl_sound_system>()));
#else
	servicelocator<Logger>::register_service_locator(new Console_Logger()); //TODO change to file logger
	servicelocator<sound_system>::register_service_locator(new sdl_sound_system);
#endif

	servicelocator<sound_system>::get_serviceLocator().AddSound(0, "../Data/Sounds/Shoot.wav"); //TODO file path needs to be different than for example sprites path?


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
		mapComponent->Initialize("../Data/Level/LevelLayout0.csv", 8);
	}


	auto tank = std::make_shared<dae::GameObject>();
	{
		tank->Initialize("Tank", &scene);
		scene.Add(tank);
		
		tank->renderer()->SetTexture("Sprites/BlueTank.png");
		tank->transform()->SetLocalPosition({250, 250 });
		HealthComponent* pHealth = tank->AddComponent<dae::HealthComponent>();

		auto rb = tank->AddComponent<dae::RigidbodyComponent>();
		
		auto collider = tank->AddComponent<dae::BoxColliderComponent>();
		collider->Initialize(6, 6, 20, 20);
		collider->AddObserver(rb);

		pHealth->AddObserver(pAchiementObserver.get());
		pHealth->AddObserver(pRemainingLives);

		pHealth->SetMaxLives(3);
		pHealth->SetMaxHealth(1);

		pHealth->Damage(2);

		ScoreComponent* pScore = tank->AddComponent<dae::ScoreComponent>();

		pScore->AddObserver(pScoreTXT);
		pScore->AddScore(10);

		tank->AddComponent<dae::GunComponent>();

	}

	auto TitleGo = std::make_shared<GameObject>();
	{
		TitleGo->Initialize("Text_GB", &scene);

		auto textcomp = TitleGo->AddComponent<TextComponent>();

		textcomp->Initialize("Tron battle tanks", font);

		TitleGo->transform()->SetLocalPosition({ 80, 20 });
		scene.Add(TitleGo);
	}

	std::vector<unsigned int> ShootInput
	{
		Controller::GamepadInput::A
	};

	std::vector<unsigned int> MoveInput
	{
		Controller::GamepadInput::LEFT_THUMB
	};

	auto player1_ShootCommand{ std::make_unique<dae::ShootCommand>(tank.get()) };
	auto player1_MoveCommand{ std::make_unique<dae::GridMoveCommand>(tank.get(), 50.f) };
	InputManager::GetInstance().BindCommand(ShootInput, InputManager::InputType::OnButtonDown, std::move(player1_ShootCommand), 0);
	InputManager::GetInstance().BindCommand(MoveInput, InputManager::InputType::OnAnalog, std::move(player1_MoveCommand), 0);
}


int main(int, char* []) 
{
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}