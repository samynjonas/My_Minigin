#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <memory>
#include <iostream>
#include <string>

#include "sdl.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Gamestatemachine.h"

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
#include "LeaderbordComponent.h"
#include "TextInputComponent.h"

#include "servicelocator.h"
#include "sound_system.h"
#include "sdl_sound_system.h"
#include "Loggin_Sound_System.h"

#include "ScoreSaver.h"

#include "Logger.h"
#include "Console_Logger.h"

using namespace dae;

void load()
{

#if _DEBUG
	servicelocator<Logger>::register_service_locator(std::make_unique<Console_Logger>());
	servicelocator<sound_system>::register_service_locator(std::make_unique<Loggin_Sound_System>(std::make_unique<sdl_sound_system>()));

#else
	servicelocator<Logger>::register_service_locator(std::make_unique<Console_Logger>()); //TODO change to file logger
	servicelocator<sound_system>::register_service_locator(std::make_unique<sdl_sound_system>());
#endif

	servicelocator<sound_system>::get_serviceLocator().AddSound("Shoot", 64, "../Data/Sounds/Shoot.wav");

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	//Scene controller can be used to go to next or previous scene
	auto sceneController = std::make_shared<dae::GameObject>();
	{
		sceneController->Initialize("Scene controller", nullptr);

		auto nextScene{ std::make_unique<dae::LoadNextSceneCommand>(sceneController.get()) };
		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_F4, InputManager::InputType::OnButtonDown, std::move(nextScene));

		auto prevScene{ std::make_unique<dae::LoadPreviousSceneCommand>(sceneController.get()) };
		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_F3, InputManager::InputType::OnButtonDown, std::move(prevScene));

		auto musicMute{ std::make_unique<dae::MuteCommand>(sceneController.get()) };
		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_M, InputManager::InputType::OnButtonDown, std::move(musicMute));
	}


	auto& MainMenuScene = dae::SceneManager::GetInstance().CreateScene("MainMenu");
	{
		//Show start game
		// Select gamemode
		//Ask for controls
		//Quit game

		auto StartGameText = std::make_shared<dae::GameObject>();
		{
			MainMenuScene.Add(StartGameText);

			StartGameText->Initialize("StartGameText", &MainMenuScene);
			auto textComp = StartGameText->AddComponent<TextComponent>();
			textComp->Initialize("A/SPACE to start", font);
			StartGameText->transform()->SetLocalPosition({150.f, 100.f});
		}

		auto QuitGameText = std::make_shared<dae::GameObject>();
		{
			MainMenuScene.Add(QuitGameText);

			QuitGameText->Initialize("QuitGameText", &MainMenuScene);
			auto textComp = QuitGameText->AddComponent<TextComponent>();
			textComp->Initialize("B/ESC to quit", font);
			QuitGameText->transform()->SetLocalPosition({ 150.f, 200.f });
		}

		auto ChangeGamemodeText = std::make_shared<dae::GameObject>();
		{
			MainMenuScene.Add(ChangeGamemodeText);

			ChangeGamemodeText->Initialize("StartGameText", &MainMenuScene);
			auto textComp = ChangeGamemodeText->AddComponent<TextComponent>();
			textComp->Initialize("T/Tab to Change", font);
			ChangeGamemodeText->transform()->SetLocalPosition({ 150.f, 300.f });

			ChangeGamemodeText->AddComponent<TextInputComponent>();
		}

		auto loadGameKeyboard{		std::make_unique<dae::LoadSceneCommand>(StartGameText.get(), "SceneMap1") };
		InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_SPACE, InputManager::InputType::OnButtonDown, std::move(loadGameKeyboard));

	}

	auto& sceneLevel1 = dae::SceneManager::GetInstance().CreateScene("SceneMap1");
	{
		//Spawn everything in the map
		auto map_1 = std::make_shared<dae::GameObject>();
		map_1->Initialize("Map_1", &sceneLevel1);
		sceneLevel1.Add(map_1);

		map_1->transform()->SetLocalPosition({ 50.f, 0.f });

		auto mapComponent = map_1->AddComponent<dae::MapGeneratorComponent>();
		mapComponent->Initialize("../Data/Level/LevelLayout1.csv", 16);

		sceneLevel1.AddObserver(mapComponent, { LevelLoad, LevelUnload });
	}

	auto& sceneLevel2 = dae::SceneManager::GetInstance().CreateScene("SceneMap2");
	{
		//Spawn everything in the map
		auto map_1 = std::make_shared<dae::GameObject>();
		map_1->Initialize("Map_2", &sceneLevel2);
		sceneLevel2.Add(map_1);

		map_1->transform()->SetLocalPosition({ 50.f, 0.f });

		auto mapComponent = map_1->AddComponent<dae::MapGeneratorComponent>();
		mapComponent->Initialize("../Data/Level/LevelLayout2.csv", 16);

		sceneLevel2.AddObserver(mapComponent, { LevelLoad, LevelUnload });
	}

	auto& sceneLevel3 = dae::SceneManager::GetInstance().CreateScene("SceneMap3");
	{
		//Spawn everything in the map
		auto map_1 = std::make_shared<dae::GameObject>();
		map_1->Initialize("Map_3", &sceneLevel3);
		sceneLevel3.Add(map_1);

		map_1->transform()->SetLocalPosition({ 50.f, 0.f });

		auto mapComponent = map_1->AddComponent<dae::MapGeneratorComponent>();
		mapComponent->Initialize("../Data/Level/LevelLayout3.csv", 16);

		sceneLevel3.AddObserver(mapComponent, { LevelLoad, LevelUnload });
	}

	auto& sceneLevel1_coop = dae::SceneManager::GetInstance().CreateScene("SceneMap1_Coop");
	{
		//Spawn everything in the map
		auto map_1 = std::make_shared<dae::GameObject>();
		map_1->Initialize("Map_1", &sceneLevel1_coop);
		sceneLevel1_coop.Add(map_1);

		map_1->transform()->SetLocalPosition({ 50.f, 0.f });

		auto mapComponent = map_1->AddComponent<dae::MapGeneratorComponent>();
		mapComponent->Initialize("../Data/Level/LevelLayout1.csv", 16);

		sceneLevel1_coop.AddObserver(mapComponent, { LevelLoad, LevelUnload });
	}

	auto& sceneLevel2_coop = dae::SceneManager::GetInstance().CreateScene("SceneMap2_Coop");
	{
		//Spawn everything in the map
		auto map_1 = std::make_shared<dae::GameObject>();
		map_1->Initialize("Map_2", &sceneLevel2_coop);
		sceneLevel2_coop.Add(map_1);

		map_1->transform()->SetLocalPosition({ 50.f, 0.f });

		auto mapComponent = map_1->AddComponent<dae::MapGeneratorComponent>();
		mapComponent->Initialize("../Data/Level/LevelLayout2.csv", 16);

		sceneLevel2_coop.AddObserver(mapComponent, { LevelLoad, LevelUnload });
	}

	auto& sceneLevel3_coop = dae::SceneManager::GetInstance().CreateScene("SceneMap3_Coop");
	{
		//Spawn everything in the map
		auto map_1 = std::make_shared<dae::GameObject>();
		map_1->Initialize("Map_3", &sceneLevel3_coop);
		sceneLevel3_coop.Add(map_1);

		map_1->transform()->SetLocalPosition({ 50.f, 0.f });

		auto mapComponent = map_1->AddComponent<dae::MapGeneratorComponent>();
		mapComponent->Initialize("../Data/Level/LevelLayout3.csv", 16);

		sceneLevel3_coop.AddObserver(mapComponent, { LevelLoad, LevelUnload });
	}

	auto& HighScoreScene = dae::SceneManager::GetInstance().CreateScene("HighScoreMenu");
	{
		auto leaderBord = std::make_shared<dae::GameObject>();
		{
			HighScoreScene.Add(leaderBord);

			leaderBord->Initialize("leaderBord", &HighScoreScene);
			auto leaderbordComp = leaderBord->AddComponent<LeaderbordComponent>();
			leaderbordComp->DisplayLeaderbord();
		}
	}

	Gamestatemachine::GetInstance().Start();
}


int main(int, char* []) 
{
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}