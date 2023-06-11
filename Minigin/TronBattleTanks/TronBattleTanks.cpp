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

#include "servicelocator.h"
#include "sound_system.h"
#include "sdl_sound_system.h"
#include "Loggin_Sound_System.h"

#include "Logger.h"
#include "Console_Logger.h"

using namespace dae;

void load()
{

#if _DEBUG
	servicelocator<Logger>::register_service_locator(std::make_unique<Console_Logger>());
	servicelocator<sound_system>::register_service_locator(std::make_unique<Loggin_Sound_System>(std::make_unique<sdl_sound_system>()));

#else
	servicelocator<Logger>::register_service_locator(new Console_Logger()); //TODO change to file logger
	servicelocator<sound_system>::register_service_locator(new sdl_sound_system);
#endif

	servicelocator<sound_system>::get_serviceLocator().AddSound("Shoot", 64, "../Data/Sounds/Shoot.wav");

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	//Scene controller can be used to go to next or previous scene
	auto sceneController = std::make_shared<dae::GameObject>();
	{
		sceneController->Initialize("Scene controller", nullptr);

		auto nextScene{ std::make_unique<dae::LoadNextSceneCommand>(sceneController.get()) };
		InputManager::GetInstance().BindCommand(SDL_SCANCODE_F4, InputManager::InputType::OnButtonUp, std::move(nextScene), 0);

		auto prevScene{ std::make_unique<dae::LoadPreviousSceneCommand>(sceneController.get()) };
		InputManager::GetInstance().BindCommand(SDL_SCANCODE_F3, InputManager::InputType::OnButtonUp, std::move(prevScene), 0);
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
		}

		auto loadGameController{	std::make_unique<dae::LoadSceneCommand>(StartGameText.get(), "SceneMap1") };
		auto loadGameKeyboard{		std::make_unique<dae::LoadSceneCommand>(StartGameText.get(), "SceneMap1") };

		InputManager::GetInstance().BindCommand(SDL_SCANCODE_SPACE, InputManager::InputType::OnButtonDown, std::move(loadGameController), 0);
		InputManager::GetInstance().BindCommand(Controller::GamepadInput::A, InputManager::InputType::OnButtonDown, std::move(loadGameKeyboard), 0);

	}	

	auto& sceneLevel1 = dae::SceneManager::GetInstance().CreateScene("SceneMap1");
	{
		auto remainingLivesGO = std::make_shared<dae::GameObject>();
		sceneLevel1.Add(remainingLivesGO);
		remainingLivesGO->Initialize("Lives", &sceneLevel1);
		TextComponent* pRemainingLives = remainingLivesGO->AddComponent<dae::TextComponent>();
		pRemainingLives->Initialize("Lives: 3", font);
		remainingLivesGO->transform()->SetLocalPosition({ 5, 380 });

		auto scoreGO = std::make_shared<dae::GameObject>();
		scoreGO->Initialize("Score", &sceneLevel1);
		TextComponent* pScoreTXT = scoreGO->AddComponent<dae::TextComponent>();
		pScoreTXT->Initialize("Score: 0", font);

		auto score = scoreGO->AddComponent<ScoreComponent>();
		score->AddObserver(pScoreTXT, { ScoreUpdated });

		scoreGO->transform()->SetLocalPosition({ 5, 400 });
		sceneLevel1.Add(scoreGO);

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
		auto remainingLivesGO = std::make_shared<dae::GameObject>();
		sceneLevel2.Add(remainingLivesGO);
		remainingLivesGO->Initialize("Lives", &sceneLevel2);
		TextComponent* pRemainingLives = remainingLivesGO->AddComponent<dae::TextComponent>();
		pRemainingLives->Initialize("Lives: 3", font);
		remainingLivesGO->transform()->SetLocalPosition({ 5, 380 });

		auto scoreGO = std::make_shared<dae::GameObject>();
		scoreGO->Initialize("Score", &sceneLevel2);
		TextComponent* pScoreTXT = scoreGO->AddComponent<dae::TextComponent>();
		pScoreTXT->Initialize("Score: 0", font);

		auto score = scoreGO->AddComponent<ScoreComponent>();
		score->AddObserver(pScoreTXT, { ScoreUpdated });

		scoreGO->transform()->SetLocalPosition({ 5, 400 });
		sceneLevel2.Add(scoreGO);

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
		auto remainingLivesGO = std::make_shared<dae::GameObject>();
		sceneLevel3.Add(remainingLivesGO);
		remainingLivesGO->Initialize("Lives", &sceneLevel3);
		TextComponent* pRemainingLives = remainingLivesGO->AddComponent<dae::TextComponent>();
		pRemainingLives->Initialize("Lives: 3", font);
		remainingLivesGO->transform()->SetLocalPosition({ 5, 380 });

		auto scoreGO = std::make_shared<dae::GameObject>();
		scoreGO->Initialize("Score", &sceneLevel3);
		TextComponent* pScoreTXT = scoreGO->AddComponent<dae::TextComponent>();
		pScoreTXT->Initialize("Score: 0", font);

		auto score = scoreGO->AddComponent<ScoreComponent>();
		score->AddObserver(pScoreTXT, { ScoreUpdated });

		scoreGO->transform()->SetLocalPosition({ 5, 400 });
		sceneLevel3.Add(scoreGO);

		//Spawn everything in the map
		auto map_1 = std::make_shared<dae::GameObject>();
		map_1->Initialize("Map_3", &sceneLevel3);
		sceneLevel3.Add(map_1);

		map_1->transform()->SetLocalPosition({ 50.f, 0.f });

		auto mapComponent = map_1->AddComponent<dae::MapGeneratorComponent>();
		mapComponent->Initialize("../Data/Level/LevelLayout3.csv", 16);

		sceneLevel3.AddObserver(mapComponent, { LevelLoad, LevelUnload });
	}
	
	//auto& HighScoreScene = dae::SceneManager::GetInstance().CreateScene("HighScoreMenu");
	//{
	//	//Read highscores out of json
	//	//
	//}

	Gamestatemachine::GetInstance().Start();

}


int main(int, char* []) 
{
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}