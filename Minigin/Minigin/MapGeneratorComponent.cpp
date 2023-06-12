#include "MapGeneratorComponent.h"

#include "GameObject.h"
#include "Controller.h"
#include "InputManager.h"
#include "ActionCommands.h"
#include "ResourceManager.h"

#include "Scene.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"

#include "../TronBattleTanks/Gamestatemachine.h"
#include "../TronBattleTanks/CounterComponent.h"
#include "../TronBattleTanks/AI_BehaviourComponent.h"
#include "../TronBattleTanks/GunComponent.h"
#include "../TronBattleTanks/TeleportComponent.h"
#include "../TronBattleTanks/Commands.h"


#include <string>

dae::MapGeneratorComponent::MapGeneratorComponent()
	: Component()
{

}

void dae::MapGeneratorComponent::Initialize(const std::string& jsonMapFile, const float& block_size, const int playercount)
{
	MAX_PLAYERS = playercount;
	m_BlockSize = block_size;
	m_JsonMapFile = jsonMapFile;

	if (m_pScene == nullptr)
	{
		m_pScene = GetOwner()->GetScene();
	}

	m_ParentPos = GetOwner()->transform()->GetWorldPosition();
}

void dae::MapGeneratorComponent::CreateWall(int row, int coll)
{
	auto wall = std::make_shared<dae::GameObject>();
	m_pScene->Add(wall);
	
	wall->Initialize("Wall", m_pScene);
	wall->SetParent(GetOwner());
	
	wall->renderer()->SetTexture("Sprites/WallPiece.png");

	wall->transform()->SetLocalPosition( { m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize} );
	auto collider = wall->AddComponent<dae::BoxColliderComponent>();
	collider->Initialize(static_cast<int>(m_BlockSize), static_cast<int>(m_BlockSize), false, true, "Walls");
}

void dae::MapGeneratorComponent::CreateFloor(int , int )
{
	//auto floor = std::make_shared<dae::GameObject>();
	//m_pScene->Add(floor);
	//
	//floor->Initialize("Floor", m_pScene);
	//floor->renderer()->SetTexture("Sprites/FloorPiece.png");
	//floor->SetParent(GetOwner());
	//
	//floor->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });
}

void dae::MapGeneratorComponent::CreateBlueTank(int row, int coll)
{
	auto blueTank = std::make_shared<dae::GameObject>();
	
	m_pScene->Add(blueTank);
	
	blueTank->Initialize("BlueTank", m_pScene);
	blueTank->SetParent(GetOwner());

	blueTank->renderer()->SetTexture("Sprites/BlueTank.png");
	blueTank->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });

	auto rb = blueTank->AddComponent<RigidbodyComponent>();

	auto aiBehaviour = blueTank->AddComponent<AI_BehaviourComponent>();
	aiBehaviour->Initialize(30.f);

	auto score = blueTank->AddComponent<ScoreComponent>();
	score->AddScore(100);

	auto health = blueTank->AddComponent<HealthComponent>();
	health->Initialize(1, 3);

	auto collider = blueTank->AddComponent<BoxColliderComponent>();
	collider->Initialize(6, 6, 20, 20, false, false, "Enemy", { "Walls" });
	collider->AddObserver(aiBehaviour, { CollisionExit });
	collider->AddObserver(rb, { CollisionEnter, CollisionExit });

	auto trigger = blueTank->AddComponent<BoxColliderComponent>();
	trigger->Initialize(true, false, "Enemy", { "Friendly" });
	trigger->AddObserver(health, { TriggerEnter });

	auto gun = std::make_shared<dae::GameObject>();
	m_pScene->Add(gun);

	gun->Initialize("RedTank_Gun", m_pScene);

	gun->renderer()->SetTexture("Sprites/RedTankGun.png");
	gun->SetParent(blueTank.get());

	auto tankTextureDim = blueTank->renderer()->GetTextureDimensions();
	gun->transform()->SetLocalPosition({ -tankTextureDim.x / 2, -tankTextureDim.y / 2 });

	auto gunComponent = blueTank->AddComponent<dae::GunComponent>();
	gunComponent->Initialize("Enemy", { "Player" }, 150, 2.5f);

	m_VecEnemies.push_back(blueTank.get());
}

void dae::MapGeneratorComponent::CreatePlayer(int row, int coll)
{
	if (m_VecPlayers.size() >= MAX_PLAYERS)
	{
		CreateFloor(row, coll);
		return;
	}

	auto tank = std::make_shared<dae::GameObject>();
	m_pScene->Add(tank);

	tank->Initialize("Player", m_pScene);
	tank->SetParent(GetOwner());

	tank->renderer()->SetTexture("Sprites/RedTank.png");
	tank->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });

	auto health = tank->AddComponent<dae::HealthComponent>();
	health->Initialize(1, 3);

	auto rb = tank->AddComponent<dae::RigidbodyComponent>();

	auto collider = tank->AddComponent<dae::BoxColliderComponent>();
	collider->Initialize(6, 6, 20, 20, false, false, "Player", { "Walls" });
	collider->AddObserver(rb, { CollisionEnter, CollisionExit });

	auto gun = std::make_shared<dae::GameObject>();
	m_pScene->Add(gun);

	gun->Initialize("RedTank_Gun", m_pScene);

	gun->renderer()->SetTexture("Sprites/RedTankGun.png");
	gun->SetParent(tank.get());

	auto tankTextureDim = tank->renderer()->GetTextureDimensions();
	gun->transform()->SetLocalPosition({ -tankTextureDim.x / 2, -tankTextureDim.y / 2 });

	auto teleporter = tank->AddComponent<dae::TeleportComponent>();
	teleporter->Initialize({static_cast<int>(m_ParentPos.x + m_BlockSize), static_cast<int>(m_ParentPos.y + m_BlockSize), m_Width, m_Height });

	auto trigger = tank->AddComponent<dae::BoxColliderComponent>();
	trigger->Initialize(true, false, "PlayerTrigger", { "Teleporter" });
	trigger->AddObserver(teleporter, { TriggerEnter });

	auto trigger_2 = tank->AddComponent<dae::BoxColliderComponent>();
	trigger_2->Initialize(true, false, "Player", { "Enemy" });
	trigger_2->AddObserver(health, { TriggerEnter });

	auto gunComponent = gun->AddComponent<dae::GunComponent>();
	gunComponent->Initialize("Player", { "Enemy" }, 150.f, 1.5f);

	auto player1_MoveCommand{ std::make_unique<dae::GridMoveCommand>(tank.get(), 50.f) };
	InputManager::GetInstance().BindCommand(Controller::GamepadInput::LEFT_THUMB, InputManager::InputType::OnAnalog, std::move(player1_MoveCommand), static_cast<int>(m_VecPlayers.size()));
	
	auto player1_ShootCommand{ std::make_unique<dae::ShootCommand>(gun.get())	};
	InputManager::GetInstance().BindCommand(Controller::GamepadInput::A, InputManager::InputType::OnButtonDown, std::move(player1_ShootCommand), static_cast<int>(m_VecPlayers.size()));

	auto player1_GunRotationCommand{ std::make_unique<dae::RotationCommand>(gun.get(), 50.f) };
	InputManager::GetInstance().BindCommand(Controller::GamepadInput::RIGHT_THUMB,	InputManager::InputType::OnAnalog, std::move(player1_GunRotationCommand), static_cast<int>(m_VecPlayers.size()));

	m_VecPlayers.push_back(tank.get());
}

void dae::MapGeneratorComponent::CreateTeleporter(int row, int coll)
{
	auto teleporter = std::make_shared<dae::GameObject>();
	m_pScene->Add(teleporter);
	
	teleporter->Initialize("Teleporter", m_pScene);
	
	teleporter->renderer()->SetTexture("Sprites/Teleporter.png");
	teleporter->SetParent(GetOwner());
	
	teleporter->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });

	auto trigger = teleporter->AddComponent<dae::BoxColliderComponent>();
	trigger->Initialize(true, false, "Teleporter", { "PlayerTrigger" });
}

void dae::MapGeneratorComponent::CreateRecognizer(int row, int coll)
{
	auto recognizer = std::make_shared<dae::GameObject>();
	m_pScene->Add(recognizer);

	recognizer->Initialize("Recognizer", m_pScene);
	recognizer->SetParent(GetOwner());

	recognizer->renderer()->SetTexture("Sprites/Recognizer.png");
	recognizer->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });

	auto rb = recognizer->AddComponent<RigidbodyComponent>();

	auto aiBehaviour = recognizer->AddComponent<AI_BehaviourComponent>();
	aiBehaviour->Initialize(60.f);

	auto collider = recognizer->AddComponent<BoxColliderComponent>();
	collider->Initialize(6, 6, 20, 20, false, false, "Enemy", { "Walls" });
	collider->AddObserver(aiBehaviour, { CollisionEnter, CollisionExit });
	collider->AddObserver(rb, { CollisionEnter, CollisionExit });

	m_VecEnemies.push_back(recognizer.get());
}

void dae::MapGeneratorComponent::LoadMap()
{
	auto parsedVector = m_CSVParser.ParseCSV(m_JsonMapFile);

	m_Height = (static_cast<int>(parsedVector.size()) - 1) * static_cast<int>(m_BlockSize);
	m_Width = (static_cast<int>(parsedVector[0].size()) - 1) * static_cast<int>(m_BlockSize);

	for (int row{}; row < parsedVector.size(); ++row)
	{
		for (int coll{}; coll < parsedVector[row].size(); ++coll)
		{
			//Depending on the float value
			switch (static_cast<MapPiece>(parsedVector[row][coll]))
			{
			case MapPiece::WallNoCollider:
				CreateWall(row, coll);
				break;
			case MapPiece::Wall:
				CreateWall(row, coll);
				break;
			case MapPiece::Floor:
				CreateFloor(row, coll);
				break;
			case MapPiece::Diamond:
				CreateTeleporter(row, coll);
				break;
			case MapPiece::BlueTankSpawner:
				CreateBlueTank(row, coll);
				break;
			case MapPiece::RecognizerSpawner:
				CreateRecognizer(row, coll);
				break;
			case MapPiece::PlayerSpawner:
				CreatePlayer(row, coll);
				break;
			default:
				break;
			}
		}
	}

	auto enemyCounter = GetOwner()->AddComponent<CounterComponent>();
	enemyCounter->Initialize( static_cast<int>(m_VecEnemies.size()));

	auto globalscore = GetOwner()->GetComponent<ScoreComponent>();
	if (globalscore == nullptr)
	{
		globalscore = GetOwner()->AddComponent<ScoreComponent>();
	}

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto scoreText = std::make_shared<dae::GameObject>();
	{
		m_pScene->Add(scoreText);
		scoreText->Initialize("Score", m_pScene);

		TextComponent* pScoreTXT = scoreText->AddComponent<dae::TextComponent>();
		pScoreTXT->Initialize("Score: 0", font);

		globalscore->AddObserver(pScoreTXT, { ScoreUpdated });

		scoreText->transform()->SetLocalPosition({ 5, 400 });
	}

	auto playerLives = std::make_shared<dae::GameObject>();
	TextComponent* pRemainingLives{ nullptr };
	{
		m_pScene->Add(playerLives);
		playerLives->Initialize("Lives", m_pScene);

		pRemainingLives = playerLives->AddComponent<dae::TextComponent>();
		pRemainingLives->Initialize("Lives: 3", font);
		playerLives->transform()->SetLocalPosition({ 5, 380 });
	}


	for (auto& enemy : m_VecEnemies)
	{
		auto health = enemy->GetComponent<HealthComponent>();
		if (health)
		{
			health->AddObserver(enemyCounter, { ObjectDied });
			health->AddObserver(globalscore, { ObjectDied });
		}
	}
	enemyCounter->AddObserver(&Gamestatemachine::GetInstance(), { CounterFinished });

	for (auto& player : m_VecPlayers)
	{
		auto health = player->GetComponent<HealthComponent>();
		if (health)
		{
			//health->AddObserver(&Gamestatemachine::GetInstance(), { ObjectDied, LiveLost });
			health->AddObserver(pRemainingLives, { LiveLost });
		}
	}
}

void dae::MapGeneratorComponent::UnloadMap()
{
	for (auto& child : GetOwner()->GetChildren())
	{
		if (child)
		{
			child->MarkForDead();
		}
	}

	m_VecPlayers.clear();
	m_VecEnemies.clear();
}

void dae::MapGeneratorComponent::Notify(Event currEvent, subject*)
{
	if (currEvent == LevelLoad)
	{
		LoadMap();
	}
	else if (currEvent == LevelUnload)
	{
		UnloadMap();
	}
}