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

#include "GameObject.h"
#include "Controller.h"
#include "InputManager.h"
#include "ActionCommands.h"

#include "Bomberman_Commands.h"
#include "BombermanBehaviourComponent.h"
#include "MapGeneratorComponent.h"
#include "CameraComponent.h"

#include "servicelocator.h"
#include "sound_system.h"
#include "sdl_sound_system.h"
#include "Loggin_Sound_System.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"
#include "AIBehaviourComponent.h"
#include "EnemySpawnerComponent.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "CounterComponent.h"
#include "LeaderbordComponent.h"
#include "GameModePickerComponent.h"
#include "ButtonComponent.h"

#include "Logger.h"
#include "Console_Logger.h"

#include "GameModes.h"

using namespace dae;

void load()
{

#if _DEBUG
	servicelocator<Logger>::register_service_locator(std::make_unique<Console_Logger>());
	servicelocator<sound_system>::register_service_locator(std::make_unique<Loggin_Sound_System>(std::make_unique<sdl_sound_system>()));

#else
	servicelocator<Logger>::register_service_locator(std::make_unique<Console_Logger>());
	servicelocator<sound_system>::register_service_locator(std::make_unique<sdl_sound_system>());
#endif
	servicelocator<sound_system>::get_serviceLocator().AddSound("BombExplosion", 64, "../Data/Sounds/Bomberman/Bomb_Explosion.wav");
	servicelocator<sound_system>::get_serviceLocator().AddSound("Powerup", 64, "../Data/Sounds/Bomberman/Detonator.wav");
	servicelocator<sound_system>::get_serviceLocator().AddSound("MoreFlames", 64, "../Data/Sounds/Bomberman/MoreFlames.wav");
	servicelocator<sound_system>::get_serviceLocator().AddSound("Level_End", 64, "../Data/Sounds/Bomberman/Level_End.wav");

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);

	auto skipScene{ std::make_unique <dae::LoadNextSceneCommand> (nullptr) };
	InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_F4, dae::InputManager::InputType::OnButtonUp, std::move(skipScene));

	auto prevScene{ std::make_unique <dae::LoadNextSceneCommand>(nullptr) };
	InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_F3, dae::InputManager::InputType::OnButtonUp, std::move(prevScene));

	auto muteSound{ std::make_unique <dae::MuteCommand>(nullptr) };
	InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_F2, dae::InputManager::InputType::OnButtonUp, std::move(muteSound));


	auto& StartScene = dae::SceneManager::GetInstance().CreateScene("StartScene");
	{
		auto GameModePicker = std::make_unique<dae::GameObject>();
		{
			auto pGameModePicker = GameModePicker.get();
			StartScene.Add(std::move(GameModePicker));

			pGameModePicker->Initialize("GameModePicker", &StartScene);
			pGameModePicker->transform()->SetLocalPosition({ 120, 200 });

			auto gameModePickerComp = pGameModePicker->AddComponent<GameModePickerComponent>();
			{
				auto singlePlayer = std::make_unique<dae::GameMode_SinglePlayer>();
				auto coop = std::make_unique<dae::GameMode_Coop>();
				auto versus = std::make_unique<dae::GameMode_Versus>();

				gameModePickerComp->AddGameMode(std::move(singlePlayer),	"SinglePlayer");
				gameModePickerComp->AddGameMode(std::move(coop),			"Co-Op");
				gameModePickerComp->AddGameMode(std::move(versus),			"Versus");
			}

			auto textComponent = pGameModePicker->AddComponent<TextComponent>();
			{
				textComponent->Initialize("Single", font, { 255, 255, 255 }, 6);
				textComponent->SetGetterFunction([gameModePickerComp] { return gameModePickerComp->GetGameModeNames(); });
			}

			auto button = pGameModePicker->AddComponent<ButtonComponent>();
			button->Select();
			button->AddObserver(gameModePickerComp, { ButtonPressed });

			//Controller Inputs
			{
				auto ButtonPress{ std::make_unique<dae::PressButton>(pGameModePicker) };
				InputManager::GetInstance().BindCommand(Controller::GamepadInput::A, InputManager::InputType::OnButtonDown, std::move(ButtonPress), 0);

				auto cyclePicker{ std::make_unique<dae::CyclePicker>(pGameModePicker) };
				InputManager::GetInstance().BindCommand(Controller::GamepadInput::DPAD_RIGHT, InputManager::InputType::OnButtonDown, std::move(cyclePicker), 0);
			}

			//Keyboard inputs
			{
				auto ButtonPress_Keyboard{ std::make_unique<dae::PressButton>(pGameModePicker) };
				InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_SPACE, InputManager::InputType::OnButtonUp, std::move(ButtonPress_Keyboard));

				auto cyclePicker_Keyboard{ std::make_unique<dae::CyclePicker>(pGameModePicker) };
				InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, InputManager::InputType::OnButtonDown, std::move(cyclePicker_Keyboard));
			}			
		}

		//auto LeaderbordButton = std::make_unique < dae::GameObject>();
		//{
		//	auto pLeaderbordbutton = LeaderbordButton.get();
		//	StartScene.Add(std::move(LeaderbordButton));
		//
		//	pLeaderbordbutton->Initialize("LeaderbordButton", &StartScene);
		//	pLeaderbordbutton->transform()->SetLocalPosition({120, 250});
		//
		//	pLeaderbordbutton->AddComponent<ButtonComponent>();
		//
		//	auto textComponent = pLeaderbordbutton->AddComponent<TextComponent>();
		//	{
		//		textComponent->Initialize("Leaderbord", font, { 255, 255, 255 }, 6);
		//	}
		//
		//	auto ButtonPress{ std::make_unique<dae::PressButton>(pLeaderbordbutton) };
		//	InputManager::GetInstance().BindCommand(Controller::GamepadInput::A, InputManager::InputType::OnButtonDown, std::move(ButtonPress), 0);
		//
		//	auto ButtonPress_Keyboard{ std::make_unique<dae::PressButton>(pLeaderbordbutton) };
		//	InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_SPACE, InputManager::InputType::OnButtonUp, std::move(ButtonPress_Keyboard));
		//}
	}
	
	auto& LeaderbordScene = dae::SceneManager::GetInstance().CreateScene("LeaderbordScene");
	{
		auto leaderBord = std::make_unique<dae::GameObject>();
		{
			auto pLeaderbord = leaderBord.get();
			LeaderbordScene.Add(std::move(leaderBord));

			pLeaderbord->Initialize("leaderBord", &LeaderbordScene);

			auto leaderbordComp = pLeaderbord->AddComponent<LeaderbordComponent>();
			leaderbordComp->DisplayLeaderbord();
		}
	}

	SceneManager::GetInstance().LoadScene("StartScene");
}


int main(int, char* [])
{
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}