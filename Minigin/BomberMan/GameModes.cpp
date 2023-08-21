#include "GameModes.h"

#include "ResourceManager.h"
#include "InputManager.h"

#include "GameObject.h"
#include "Scene.h"
#include "ActionCommands.h"
#include "Bomberman_Commands.h"

#include "MapGeneratorComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "CounterComponent.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "BombermanBehaviourComponent.h"
#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"
#include "CameraComponent.h"
#include "EnemySpawnerComponent.h"

#include "GameStateManagerComponent.h"
#include "GameListener.h"

void dae::GameModes::InitializeScene()
{
	auto& gameScene1 = dae::SceneManager::GetInstance().CreateScene("GameScene_1");
	auto& gameScene2 = dae::SceneManager::GetInstance().CreateScene("GameScene_2");
	auto& gameScene3 = dae::SceneManager::GetInstance().CreateScene("GameScene_3");

	//GameState init
	SetGameState(&gameScene1, "GameScene_2",	m_pGameStateManager_1);
	SetGameState(&gameScene2, "GameScene_3",	m_pGameStateManager_2);
	SetGameState(&gameScene3, "",				m_pGameStateManager_3);

	//The same for all the levels
	SpawnMap(&gameScene1);
	SpawnMap(&gameScene2);
	SpawnMap(&gameScene3);

	//The same for all the levels
	SpawnPlayers(&gameScene1);
	SpawnPlayers(&gameScene2);
	SpawnPlayers(&gameScene3);

	//Will be different - higher level = more enemies
	SpawnEnemies(&gameScene1, 1);
	SpawnEnemies(&gameScene2, 2);
	SpawnEnemies(&gameScene3, 3);

	//Setting Scene 1 as active scene
	SceneManager::GetInstance().LoadScene("GameScene_1");
}

void dae::GameModes::SpawnMap(Scene* pScene)
{
	const int GRID_SIZE{ 32 };

	//Map Loading
	auto world = std::make_unique <dae::GameObject>();
	{
		auto pWorld = world.get();

		pScene->Add(std::move(world));

		pWorld->Initialize("Map", pScene);

		auto mapComp = pWorld->AddComponent<MapGeneratorComponent>();
		mapComp->Initialize("../Data/Level/Bomberman/Bomberman_Map_Layout.csv", GRID_SIZE);
		mapComp->LoadMap();
	}
}

void dae::GameModes::SetGameState(Scene* pScene, std::string NextScene, GameStateManagerComponent* pGameStateManagerComponent)
{
	auto gameStateManager = std::make_unique <dae::GameObject>();
	{
		auto pGameStateManager = gameStateManager.get();

		pScene->Add(std::move(gameStateManager));

		pGameStateManager->Initialize("GameStateManager", pScene);

		pGameStateManagerComponent = pGameStateManager->AddComponent<GameStateManagerComponent>();
		pGameStateManagerComponent->Initialize(pScene->GetSceneName(), NextScene, "StartScene", "LeaderbordScene");
	}
}

// -------------------------- SINGLEPLAYER ----------------------------- \\

void dae::GameMode_SinglePlayer::SpawnPlayers(Scene* pScene)
{
	const int GRID_SIZE{ 32 };
	const float Player_MoveSpeed{ 35.f };
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 35);

	//HUD -- connected to player
	auto HUD = std::make_unique<dae::GameObject>();
	auto pHUD = HUD.get();
	{
		pScene->Add(std::move(HUD));
		pHUD->Initialize("HUD", pScene);

		auto renderer = pHUD->AddComponent<RenderComponent>();
		renderer->Initialize(5, true);
		renderer->SetTexture("Sprites/Bomberman/Bomberman_TopHUD.png");
	}

	//Level Time Count
	auto TimeText = std::make_unique<dae::GameObject>();
	{
		auto pTimeText = TimeText.get();
		pScene->Add(std::move(TimeText));
		pTimeText->Initialize("HUD_Time", pScene);

		pTimeText->transform()->SetLocalPosition({ 128.f, 16.f });

		auto Timer = pTimeText->AddComponent<CounterComponent>();
		Timer->Initialize(256);

		auto TextComp = pTimeText->AddComponent<TextComponent>();
		TextComp->Initialize("255", font, { 255, 255, 255 }, 4);
		TextComp->SetGetterFunction([Timer] { return Timer->GetTimeToGo(); });
	}

	//Player
	auto BomberMan = std::make_unique<dae::GameObject>();
	HealthComponent* bombermanHealth{ nullptr };
	BombermanBehaviourComponent* bombermanBehaviour{ nullptr };
	auto pBomberman = BomberMan.get();
	{
		pScene->Add(std::move(BomberMan));

		pBomberman->Initialize("Bomberman", pScene);

		auto renderComp = pBomberman->AddComponent<RenderComponent>();
		if (renderComp)
		{
			renderComp->Initialize(3);
			renderComp->SetTexture("Sprites/Bomberman/Bomberman_Man.png");
		}

		pBomberman->transform()->SetLocalPosition({ 32.f, 96.f });

		bombermanBehaviour = pBomberman->AddComponent<BombermanBehaviourComponent>();
		bombermanBehaviour->Initialize(GRID_SIZE);

		pBomberman->AddComponent<RigidbodyComponent>();

		bombermanHealth = pBomberman->AddComponent<HealthComponent>();
		bombermanHealth->Initialize(1, 3);

		bombermanHealth->AddObserver(&GameListener::GetInstance(), { ObjectDied, LiveLost });

		//auto collider = pBomberman->AddComponent<BoxColliderComponent>();
		//collider->Initialize(false, false, "Player", { "Walls", "Bombs", "Breakable" });
		//collider->AddObserver(rb, { CollisionEnter, CollisionExit });

		auto trigger = pBomberman->AddComponent<BoxColliderComponent>();
		trigger->Initialize(true, false, "Player", { "Explosion", "Enemy" });
		//trigger->AddObserver(bombermanHealth, { TriggerEnter });

		//Controller Inputs
		{
			auto Bomberman_MoveCommand{ std::make_unique<dae::GridMoveCommand>(pBomberman, Player_MoveSpeed) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::LEFT_THUMB, InputManager::InputType::OnAnalog, std::move(Bomberman_MoveCommand), 0);

			auto Bomberman_PlaceBombCommand{ std::make_unique<dae::PlaceBombCommand>(pBomberman) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::A, InputManager::InputType::OnButtonDown, std::move(Bomberman_PlaceBombCommand), 0);

			auto Bomberman_DetonateCommand{ std::make_unique<dae::DetonateBomb>(pBomberman) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::B, InputManager::InputType::OnButtonDown, std::move(Bomberman_DetonateCommand), 0);
		}

		//Keyboard Inputs
		{
			auto Bomberman_MoveUp{		std::make_unique<dae::MoveInDirection>(pBomberman, Player_MoveSpeed, glm::vec2{  0.f,  -1.f }	)};
			auto Bomberman_MoveDown{	std::make_unique<dae::MoveInDirection>(pBomberman, Player_MoveSpeed, glm::vec2{  0.f,	1.f }	)};
			auto Bomberman_MoveLeft{	std::make_unique<dae::MoveInDirection>(pBomberman, Player_MoveSpeed, glm::vec2{ -1.f,	0.f }	)};
			auto Bomberman_MoveRight{	std::make_unique<dae::MoveInDirection>(pBomberman, Player_MoveSpeed, glm::vec2{  1.f,	0.f }	)};
			
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, InputManager::InputType::OnButtonDown, std::move(Bomberman_MoveUp));
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, InputManager::InputType::OnButtonDown, std::move(Bomberman_MoveDown));
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, InputManager::InputType::OnButtonDown, std::move(Bomberman_MoveLeft));
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, InputManager::InputType::OnButtonDown, std::move(Bomberman_MoveRight));

			auto Bomberman_PlaceBombCommand{ std::make_unique<dae::PlaceBombCommand>(pBomberman) };
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_SPACE, InputManager::InputType::OnButtonDown, std::move(Bomberman_PlaceBombCommand));

			auto Bomberman_DetonateCommand{ std::make_unique<dae::DetonateBomb>(pBomberman) };
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_E, InputManager::InputType::OnButtonDown, std::move(Bomberman_DetonateCommand));
		}
	}

	//Life count
	auto LifeText = std::make_unique<dae::GameObject>();
	{
		auto pLifeText = LifeText.get();
		pScene->Add(std::move(LifeText));
		pLifeText->Initialize("HUD_Lifes", pScene);

		pLifeText->transform()->SetLocalPosition({ 592.f, 16.f });

		auto TextComp = pLifeText->AddComponent<TextComponent>();
		TextComp->Initialize("255", font, { 255, 255, 255 }, 4);
		TextComp->SetGetterFunction([bombermanHealth] { return bombermanHealth->GetLives(); });
	}

	//Bomb count
	auto BombText = std::make_unique<dae::GameObject>();
	{
		auto pBombText = BombText.get();
		pScene->Add(std::move(BombText));
		pBombText->Initialize("HUD_Bombs", pScene);

		pBombText->transform()->SetLocalPosition({ 320.f, 16.f });

		auto TextComp = pBombText->AddComponent<TextComponent>();
		TextComp->Initialize("255", font, { 255, 255, 255 }, 4);
		TextComp->SetGetterFunction([bombermanBehaviour] { return bombermanBehaviour->GetBombsInInventoryCount(); });
	}

	//Camera
	auto camera = std::make_unique<dae::GameObject>();
	{
		auto pCamera = camera.get();

		pScene->Add(std::move(camera));

		pCamera->Initialize("Camera", pScene);
		auto cameraComp = pCamera->AddComponent<CameraComponent>();

		auto pCameraBehaviour = std::make_unique<HorizontalFollowingCamera>();
		pCameraBehaviour->SetTrackingObject(pBomberman);
		pCameraBehaviour->SetLevelBoundaries(0, 992);

		cameraComp->Initialize(std::move(pCameraBehaviour), 640, 480);
	}

}

void dae::GameMode_SinglePlayer::SpawnEnemies(Scene* pScene, int level)
{
	const int GRID_SIZE{ 32 };

	if (level >= 1)
	{
		auto BalloomSpawner = std::make_unique<dae::GameObject>();
		{
			auto pBalloomSpawner = BalloomSpawner.get();

			pScene->Add(std::move(BalloomSpawner));

			pBalloomSpawner->Initialize("BalloomSpawner", pScene);
			auto enemySpawner = pBalloomSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 100, 2, 1, "Sprites/Bomberman/Bomberman_Balloom.png", { 32, 96, 928, 512 }, GRID_SIZE);
		}
	}

	if (level >= 2)
	{
		auto DollSpawner = std::make_unique<dae::GameObject>();
		{
			auto pDollSpawner = DollSpawner.get();

			pScene->Add(std::move(DollSpawner));

			pDollSpawner->Initialize("DollSpawner", pScene);
			auto enemySpawner = pDollSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 400, 3, 1, "Sprites/Bomberman/Bomberman_Doll.png", { 256, 96, 928, 512 }, GRID_SIZE);
		}

		auto OnealSpawner = std::make_unique<dae::GameObject>();
		{
			auto pOnealSpawner = OnealSpawner.get();

			pScene->Add(std::move(OnealSpawner));

			pOnealSpawner->Initialize("Bomberman_Oneal", pScene);
			auto enemySpawner = pOnealSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 200, 3, 2, "Sprites/Bomberman/Bomberman_Oneal.png", { 320, 96, 928, 512 }, GRID_SIZE);
		}
	}

	if (level >= 3)
	{
		auto Minvo = std::make_unique<dae::GameObject>();
		{
			auto pDollSpawner = Minvo.get();

			pScene->Add(std::move(Minvo));

			pDollSpawner->Initialize("DollSpawner", pScene);
			auto enemySpawner = pDollSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 800, 4, 2, "Sprites/Bomberman/Bomberman_Minvo.png", { 640, 96, 928, 512 }, GRID_SIZE);
		}
	}
}

// -------------------------- CO-OP ----------------------------- \\

void dae::GameMode_Coop::SpawnPlayers(Scene* pScene)
{
	const int GRID_SIZE{ 32 };
	const float Player_MoveSpeed{ 35.f };
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 35);

	//HUD -- connected to player
	auto HUD = std::make_unique<dae::GameObject>();
	auto pHUD = HUD.get();
	{
		pScene->Add(std::move(HUD));
		pHUD->Initialize("HUD", pScene);

		auto renderer = pHUD->AddComponent<RenderComponent>();
		if (renderer)
		{
			renderer->Initialize(5, true);
			renderer->SetTexture("Sprites/Bomberman/Bomberman_TopHUD.png");
		}
	}

	//Level Time Count
	auto TimeText = std::make_unique<dae::GameObject>();
	{
		auto pTimeText = TimeText.get();
		pScene->Add(std::move(TimeText));
		pTimeText->Initialize("HUD_Time", pScene);

		pTimeText->transform()->SetLocalPosition({ 128.f, 16.f });

		auto Timer = pTimeText->AddComponent<CounterComponent>();
		Timer->Initialize(256);

		auto TextComp = pTimeText->AddComponent<TextComponent>();
		TextComp->Initialize("255", font, { 255, 255, 255 }, 4);
		TextComp->SetGetterFunction([Timer] { return Timer->GetTimeToGo(); });
	}

	//Player
	auto BomberMan = std::make_unique<dae::GameObject>();
	HealthComponent* bombermanHealth{ nullptr };
	BombermanBehaviourComponent* bombermanBehaviour{ nullptr };
	auto pBomberman = BomberMan.get();
	{
		pScene->Add(std::move(BomberMan));

		pBomberman->Initialize("Bomberman", pScene);

		auto renderComp = pBomberman->AddComponent<RenderComponent>();
		if (renderComp)
		{
			renderComp->Initialize(3);
			renderComp->SetTexture("Sprites/Bomberman/Bomberman_Man.png");
		}

		pBomberman->transform()->SetLocalPosition({ 32.f, 96.f });

		bombermanBehaviour = pBomberman->AddComponent<BombermanBehaviourComponent>();
		bombermanBehaviour->Initialize(GRID_SIZE);

		pBomberman->AddComponent<RigidbodyComponent>();

		bombermanHealth = pBomberman->AddComponent<HealthComponent>();
		bombermanHealth->Initialize(1, 3);

		//auto collider = pBomberman->AddComponent<BoxColliderComponent>();
		//collider->Initialize(false, false, "Player", { "Walls", "Bombs", "Powerup" });
		//collider->AddObserver(rb, { CollisionEnter, CollisionExit }); //<- TODO FIX COLLISIONS

		auto trigger = pBomberman->AddComponent<BoxColliderComponent>();
		trigger->Initialize(true, false, "Player", { "Explosion", "Enemy" });
		//trigger->AddObserver(bombermanHealth, { TriggerEnter });

		//Controller Inputs
		{
			auto Bomberman_MoveCommand{ std::make_unique<dae::GridMoveCommand>(pBomberman, Player_MoveSpeed) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::LEFT_THUMB, InputManager::InputType::OnAnalog, std::move(Bomberman_MoveCommand), 0);

			auto Bomberman_PlaceBombCommand{ std::make_unique<dae::PlaceBombCommand>(pBomberman) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::A, InputManager::InputType::OnButtonDown, std::move(Bomberman_PlaceBombCommand), 0);

			auto Bomberman_DetonateCommand{ std::make_unique<dae::DetonateBomb>(pBomberman) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::B, InputManager::InputType::OnButtonDown, std::move(Bomberman_DetonateCommand), 0);
		}
	}

	//Life count
	auto LifeText = std::make_unique<dae::GameObject>();
	{
		auto pLifeText = LifeText.get();
		pScene->Add(std::move(LifeText));
		pLifeText->Initialize("HUD_Lifes", pScene);

		pLifeText->transform()->SetLocalPosition({ 592.f, 16.f });

		auto TextComp = pLifeText->AddComponent<TextComponent>();
		TextComp->Initialize("255", font, { 255, 255, 255 }, 4);
		TextComp->SetGetterFunction([bombermanHealth] { return bombermanHealth->GetLives(); });
	}

	//Bomb count
	auto BombText = std::make_unique<dae::GameObject>();
	{
		auto pBombText = BombText.get();
		pScene->Add(std::move(BombText));
		pBombText->Initialize("HUD_Bombs", pScene);

		pBombText->transform()->SetLocalPosition({ 320.f, 16.f });

		auto TextComp = pBombText->AddComponent<TextComponent>();
		TextComp->Initialize("255", font, { 255, 255, 255 }, 4);
		TextComp->SetGetterFunction([bombermanBehaviour] { return bombermanBehaviour->GetBombsInInventoryCount(); });
	}

	//Camera
	auto camera = std::make_unique<dae::GameObject>();
	{
		auto pCamera = camera.get();

		pScene->Add(std::move(camera));

		pCamera->Initialize("Camera", pScene);
		auto cameraComp = pCamera->AddComponent<CameraComponent>();

		auto pCameraBehaviour = std::make_unique<HorizontalFollowingCamera>();
		pCameraBehaviour->SetTrackingObject(pBomberman);
		pCameraBehaviour->SetLevelBoundaries(0, 992);

		cameraComp->Initialize(std::move(pCameraBehaviour), 640, 480);
	}

	//Second player
	auto BomberMan_2 = std::make_unique<dae::GameObject>();
	auto pBomberman_2 = BomberMan_2.get();
	{
		pScene->Add(std::move(BomberMan_2));

		pBomberman_2->Initialize("Bomberman_2", pScene);

		auto renderComp = pBomberman_2->AddComponent<RenderComponent>();
		if (renderComp)
		{
			renderComp->Initialize(3);
			renderComp->SetTexture("Sprites/Bomberman/Bomberman_Man_2.png");
		}

		pBomberman_2->transform()->SetLocalPosition({ 416.f, 416.f });

		auto behaviour = pBomberman_2->AddComponent<BombermanBehaviourComponent>();
		behaviour->Initialize(GRID_SIZE);

		pBomberman_2->AddComponent<RigidbodyComponent>();

		auto health = pBomberman_2->AddComponent<HealthComponent>();
		health->Initialize(1, 3);
		//bombermanHealth->AddObserver(&GameListener::GetInstance(), { ObjectDied, LiveLost });

		//auto collider = pBomberman_2->AddComponent<BoxColliderComponent>();
		//collider->Initialize(false, false, "Player", { "Walls", "Bombs", "Powerup" });
		//collider->AddObserver(rb, { CollisionEnter, CollisionExit }); //<- TODO FIX COLLISIONS

		auto trigger = pBomberman_2->AddComponent<BoxColliderComponent>();
		trigger->Initialize(true, false, "Player", { "Explosion", "Enemy" });
		//trigger->AddObserver(health, { TriggerEnter });


		//Controller Inputs
		{
			auto Bomberman_MoveCommand{ std::make_unique<dae::GridMoveCommand>(pBomberman_2, Player_MoveSpeed) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::LEFT_THUMB, InputManager::InputType::OnAnalog, std::move(Bomberman_MoveCommand), 1);

			auto Bomberman_PlaceBombCommand{ std::make_unique<dae::PlaceBombCommand>(pBomberman_2) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::A, InputManager::InputType::OnButtonDown, std::move(Bomberman_PlaceBombCommand), 1);

			auto Bomberman_DetonateCommand{ std::make_unique<dae::DetonateBomb>(pBomberman_2) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::B, InputManager::InputType::OnButtonDown, std::move(Bomberman_DetonateCommand), 1);
		}

		//Keyboard Inputs
		{
			auto Bomberman_MoveUp{		std::make_unique<dae::MoveInDirection>(pBomberman_2, Player_MoveSpeed, glm::vec2{  0.f,  -1.f }) };
			auto Bomberman_MoveDown{	std::make_unique<dae::MoveInDirection>(pBomberman_2, Player_MoveSpeed, glm::vec2{  0.f,	1.f }) };
			auto Bomberman_MoveLeft{	std::make_unique<dae::MoveInDirection>(pBomberman_2, Player_MoveSpeed, glm::vec2{ -1.f,	0.f }) };
			auto Bomberman_MoveRight{	std::make_unique<dae::MoveInDirection>(pBomberman_2, Player_MoveSpeed, glm::vec2{  1.f,	0.f }) };

			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, InputManager::InputType::OnButtonDown, std::move(Bomberman_MoveUp));
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, InputManager::InputType::OnButtonDown, std::move(Bomberman_MoveDown));
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, InputManager::InputType::OnButtonDown, std::move(Bomberman_MoveLeft));
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, InputManager::InputType::OnButtonDown, std::move(Bomberman_MoveRight));

			auto Bomberman_PlaceBombCommand{ std::make_unique<dae::PlaceBombCommand>(pBomberman_2) };
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_SPACE, InputManager::InputType::OnButtonDown, std::move(Bomberman_PlaceBombCommand));

			auto Bomberman_DetonateCommand{ std::make_unique<dae::DetonateBomb>(pBomberman_2) };
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_E, InputManager::InputType::OnButtonDown, std::move(Bomberman_DetonateCommand));
		}
	}
}

void dae::GameMode_Coop::SpawnEnemies(Scene* pScene, int level)
{
	const int GRID_SIZE{ 32 };

	if (level >= 1)
	{
		auto BalloomSpawner = std::make_unique<dae::GameObject>();
		{
			auto pBalloomSpawner = BalloomSpawner.get();

			pScene->Add(std::move(BalloomSpawner));

			pBalloomSpawner->Initialize("BalloomSpawner", pScene);
			auto enemySpawner = pBalloomSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 100, 2, 1, "Sprites/Bomberman/Bomberman_Balloom.png", { 32, 96, 928, 512 }, GRID_SIZE);
		}
	}

	if (level >= 2)
	{
		auto DollSpawner = std::make_unique<dae::GameObject>();
		{
			auto pDollSpawner = DollSpawner.get();

			pScene->Add(std::move(DollSpawner));

			pDollSpawner->Initialize("DollSpawner", pScene);
			auto enemySpawner = pDollSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 400, 3, 1, "Sprites/Bomberman/Bomberman_Doll.png", { 256, 96, 928, 512 }, GRID_SIZE);
		}

		auto OnealSpawner = std::make_unique<dae::GameObject>();
		{
			auto pOnealSpawner = OnealSpawner.get();

			pScene->Add(std::move(OnealSpawner));

			pOnealSpawner->Initialize("Bomberman_Oneal", pScene);
			auto enemySpawner = pOnealSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 200, 3, 2, "Sprites/Bomberman/Bomberman_Oneal.png", { 320, 96, 928, 512 }, GRID_SIZE);
		}
	}

	if (level >= 3)
	{
		auto Minvo = std::make_unique<dae::GameObject>();
		{
			auto pDollSpawner = Minvo.get();

			pScene->Add(std::move(Minvo));

			pDollSpawner->Initialize("DollSpawner", pScene);
			auto enemySpawner = pDollSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 800, 4, 2, "Sprites/Bomberman/Bomberman_Minvo.png", { 640, 96, 928, 512 }, GRID_SIZE);
		}
	}
}

// -------------------------- VERSUS ----------------------------- \\

void dae::GameMode_Versus::SpawnPlayers(Scene* pScene)
{
	const int GRID_SIZE{ 32 };
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 35);

	//HUD -- connected to player
	auto HUD = std::make_unique<dae::GameObject>();
	auto pHUD = HUD.get();
	{
		pScene->Add(std::move(HUD));
		pHUD->Initialize("HUD", pScene);

		auto renderer = pHUD->AddComponent<RenderComponent>();
		renderer->Initialize(5, true);
		renderer->SetTexture("Sprites/Bomberman/Bomberman_TopHUD.png");
	}

	//Level Time Count
	auto TimeText = std::make_unique<dae::GameObject>();
	{
		auto pTimeText = TimeText.get();
		pScene->Add(std::move(TimeText));
		pTimeText->Initialize("HUD_Time", pScene);

		pTimeText->transform()->SetLocalPosition({ 128.f, 16.f });

		auto Timer = pTimeText->AddComponent<CounterComponent>();
		Timer->Initialize(256);

		auto TextComp = pTimeText->AddComponent<TextComponent>();
		TextComp->Initialize("255", font, { 255, 255, 255 }, 4);
		TextComp->SetGetterFunction([Timer] { return Timer->GetTimeToGo(); });
	}

	//Player
	auto BomberMan = std::make_unique<dae::GameObject>();
	HealthComponent* bombermanHealth{ nullptr };
	BombermanBehaviourComponent* bombermanBehaviour{ nullptr };
	auto pBomberman = BomberMan.get();
	{
		pScene->Add(std::move(BomberMan));

		pBomberman->Initialize("Bomberman", pScene);

		auto renderComp = pBomberman->AddComponent<RenderComponent>();
		if (renderComp)
		{
			renderComp->Initialize(3);
			renderComp->SetTexture("Sprites/Bomberman/Bomberman_Man.png");
		}

		pBomberman->transform()->SetLocalPosition({ 32.f, 96.f });

		bombermanBehaviour = pBomberman->AddComponent<BombermanBehaviourComponent>();
		bombermanBehaviour->Initialize(GRID_SIZE);

		pBomberman->AddComponent<RigidbodyComponent>();

		bombermanHealth = pBomberman->AddComponent<HealthComponent>();
		bombermanHealth->Initialize(1, 3);
		bombermanHealth->AddObserver(&GameListener::GetInstance(), { ObjectDied, LiveLost });

		//auto collider = pBomberman->AddComponent<BoxColliderComponent>();
		//collider->Initialize(false, false, "Player", { "Walls", "Bombs" });
		//collider->AddObserver(rb, { CollisionEnter, CollisionExit }); //<- TODO FIX COLLISIONS

		auto trigger = pBomberman->AddComponent<BoxColliderComponent>();
		trigger->Initialize(true, false, "Player", { "Explosion" });
		//trigger->AddObserver(bombermanHealth, { TriggerEnter });

		//Controller Inputs
		{
			auto Bomberman_MoveCommand{ std::make_unique<dae::GridMoveCommand>(pBomberman, 35.f) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::LEFT_THUMB, InputManager::InputType::OnAnalog, std::move(Bomberman_MoveCommand), 0);

			auto Bomberman_PlaceBombCommand{ std::make_unique<dae::PlaceBombCommand>(pBomberman) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::A, InputManager::InputType::OnButtonDown, std::move(Bomberman_PlaceBombCommand), 0);

			auto Bomberman_DetonateCommand{ std::make_unique<dae::DetonateBomb>(pBomberman) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::B, InputManager::InputType::OnButtonDown, std::move(Bomberman_DetonateCommand), 0);
		}
	}

	//Life count
	auto LifeText = std::make_unique<dae::GameObject>();
	{
		auto pLifeText = LifeText.get();
		pScene->Add(std::move(LifeText));
		pLifeText->Initialize("HUD_Lifes", pScene);

		pLifeText->transform()->SetLocalPosition({ 592.f, 16.f });

		auto TextComp = pLifeText->AddComponent<TextComponent>();
		TextComp->Initialize("255", font, { 255, 255, 255 }, 4);
		TextComp->SetGetterFunction([bombermanHealth] { return bombermanHealth->GetLives(); });
	}

	//Bomb count
	auto BombText = std::make_unique<dae::GameObject>();
	{
		auto pBombText = BombText.get();
		pScene->Add(std::move(BombText));
		pBombText->Initialize("HUD_Bombs", pScene);

		pBombText->transform()->SetLocalPosition({ 320.f, 16.f });

		auto TextComp = pBombText->AddComponent<TextComponent>();
		TextComp->Initialize("255", font, { 255, 255, 255 }, 4);
		TextComp->SetGetterFunction([bombermanBehaviour] { return bombermanBehaviour->GetBombsInInventoryCount(); });
	}

	//Camera
	auto camera = std::make_unique<dae::GameObject>();
	{
		auto pCamera = camera.get();

		pScene->Add(std::move(camera));

		pCamera->Initialize("Camera", pScene);
		auto cameraComp = pCamera->AddComponent<CameraComponent>();

		auto pCameraBehaviour = std::make_unique<HorizontalFollowingCamera>();
		pCameraBehaviour->SetTrackingObject(pBomberman);
		pCameraBehaviour->SetLevelBoundaries(0, 992);

		cameraComp->Initialize(std::move(pCameraBehaviour), 640, 480);
	}

	//Second player
	auto Balloom_Player = std::make_unique<dae::GameObject>();
	auto pBalloom_Player = Balloom_Player.get();
	{
		pScene->Add(std::move(Balloom_Player));

		pBalloom_Player->Initialize("Bomberman_2", pScene);

		auto renderComp = pBalloom_Player->AddComponent<RenderComponent>();
		if (renderComp)
		{
			renderComp->Initialize(3);
			renderComp->SetTexture("Sprites/Bomberman/Bomberman_Man_2.png");
		}

		pBalloom_Player->transform()->SetLocalPosition({ 416.f, 416.f });

		auto behaviour = pBalloom_Player->AddComponent<BombermanBehaviourComponent>();
		behaviour->Initialize(GRID_SIZE);

		pBalloom_Player->AddComponent<RigidbodyComponent>();

		auto health = pBalloom_Player->AddComponent<HealthComponent>();
		health->Initialize(1, 3);

		//auto collider = pBalloom_Player->AddComponent<BoxColliderComponent>();
		//collider->Initialize(false, false, "Enemy", { "Walls", "Bombs", "Powerup" });
		//collider->AddObserver(rb, { CollisionEnter, CollisionExit }); //<- TODO FIX COLLISIONS

		auto trigger = pBalloom_Player->AddComponent<BoxColliderComponent>();
		trigger->Initialize(true, false, "Enemy", { "Explosion", "Enemy" });
		//trigger->AddObserver(health, { TriggerEnter });

		//Controller Inputs
		{
			auto Balloom_MoveCommand{ std::make_unique<dae::GridMoveCommand>(pBalloom_Player, 20.f) };
			InputManager::GetInstance().BindCommand(Controller::GamepadInput::LEFT_THUMB, InputManager::InputType::OnAnalog, std::move(Balloom_MoveCommand), 1);
		}

		//Keyboard Inputs
		{
			auto Balloom_MoveUp{ std::make_unique<dae::MoveInDirection>(pBalloom_Player, 20.f, glm::vec2{  0.f,  -1.f }) };
			auto Balloom_MoveDown{ std::make_unique<dae::MoveInDirection>(pBalloom_Player, 20.f, glm::vec2{  0.f,	1.f }) };
			auto Balloom_MoveLeft{ std::make_unique<dae::MoveInDirection>(pBalloom_Player, 20.f, glm::vec2{ -1.f,	0.f }) };
			auto Balloom_MoveRight{ std::make_unique<dae::MoveInDirection>(pBalloom_Player, 20.f, glm::vec2{  1.f,	0.f }) };

			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, InputManager::InputType::OnButtonDown, std::move(Balloom_MoveUp));
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, InputManager::InputType::OnButtonDown, std::move(Balloom_MoveDown));
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, InputManager::InputType::OnButtonDown, std::move(Balloom_MoveLeft));
			InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, InputManager::InputType::OnButtonDown, std::move(Balloom_MoveRight));
		}
	}
}

void dae::GameMode_Versus::SpawnEnemies(Scene* pScene, int level)
{
	const int GRID_SIZE{ 32 };

	if (level >= 1)
	{
		auto BalloomSpawner = std::make_unique<dae::GameObject>();
		{
			auto pBalloomSpawner = BalloomSpawner.get();

			pScene->Add(std::move(BalloomSpawner));

			pBalloomSpawner->Initialize("BalloomSpawner", pScene);
			auto enemySpawner = pBalloomSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 100, 2, 1, "Sprites/Bomberman/Bomberman_Balloom.png", { 32, 96, 928, 512 }, GRID_SIZE);
		}
	}

	if (level >= 2)
	{
		auto DollSpawner = std::make_unique<dae::GameObject>();
		{
			auto pDollSpawner = DollSpawner.get();

			pScene->Add(std::move(DollSpawner));

			pDollSpawner->Initialize("DollSpawner", pScene);
			auto enemySpawner = pDollSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 400, 3, 1, "Sprites/Bomberman/Bomberman_Doll.png", { 256, 96, 928, 512 }, GRID_SIZE);
		}

		auto OnealSpawner = std::make_unique<dae::GameObject>();
		{
			auto pOnealSpawner = OnealSpawner.get();

			pScene->Add(std::move(OnealSpawner));

			pOnealSpawner->Initialize("Bomberman_Oneal", pScene);
			auto enemySpawner = pOnealSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 200, 3, 2, "Sprites/Bomberman/Bomberman_Oneal.png", { 320, 96, 928, 512 }, GRID_SIZE);
		}
	}

	if (level >= 3)
	{
		auto Minvo = std::make_unique<dae::GameObject>();
		{
			auto pDollSpawner = Minvo.get();

			pScene->Add(std::move(Minvo));

			pDollSpawner->Initialize("DollSpawner", pScene);
			auto enemySpawner = pDollSpawner->AddComponent<EnemySpawnerComponent>();
			enemySpawner->Initialize(3, 800, 4, 2, "Sprites/Bomberman/Bomberman_Minvo.png", { 640, 96, 928, 512 }, GRID_SIZE);
		}
	}
}
