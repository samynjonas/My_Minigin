#include "MapGeneratorComponent.h"

#include "GameObject.h"
#include "Controller.h"
#include "InputManager.h"
#include "ActionCommands.h"

#include "Scene.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"

//TODO what is going on here
#include "../TronBattleTanks/AI_BehaviourComponent.h"
#include "../TronBattleTanks/GunComponent.h"
#include "../TronBattleTanks/Commands.h"

#include <string>

dae::MapGeneratorComponent::MapGeneratorComponent()
	: Component()
{

}

void dae::MapGeneratorComponent::Initialize(const std::string& jsonMapFile, const float& block_size)
{
	m_BlockSize = block_size;

	if (m_pScene == nullptr)
	{
		m_pScene = GetOwner()->GetScene();
	}

	m_ParentPos = GetOwner()->transform()->GetWorldPosition();

	auto parsedVector = m_CSVParser.ParseCSV(jsonMapFile);
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
			case MapPiece::PlayerSpawner:
				CreatePlayer(row, coll);
				break;
			default:
				break;
			}
		}
	}
}


void dae::MapGeneratorComponent::CreateWall(int row, int coll)
{
	auto wall = std::make_shared<dae::GameObject>();
	m_pScene->Add(wall);
	
	wall->Initialize("Wall", m_pScene);
	wall->renderer()->SetTexture("Sprites/WallPiece.png");
	wall->SetParent(GetOwner());

	wall->transform()->SetLocalPosition( { m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize} );
	auto collider = wall->AddComponent<dae::BoxColliderComponent>();
	collider->Initialize(static_cast<int>(m_BlockSize), static_cast<int>(m_BlockSize), true, "Walls");
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

	blueTank->renderer()->SetTexture("Sprites/BlueTank.png");
	blueTank->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });

	auto rb = blueTank->AddComponent<RigidbodyComponent>();

	auto aiBehaviour = blueTank->AddComponent<AI_BehaviourComponent>();
	aiBehaviour->Initialize(30.f);

	auto collider = blueTank->AddComponent<BoxColliderComponent>();
	collider->Initialize(6, 6, 20, 20, false, "Enemy", { "Walls" });
	collider->AddObserver(aiBehaviour);
	collider->AddObserver(rb);

}

void dae::MapGeneratorComponent::CreatePlayer(int row, int coll)
{
	auto tank = std::make_shared<dae::GameObject>();
	m_pScene->Add(tank);

	tank->Initialize("Player", m_pScene);

	tank->renderer()->SetTexture("Sprites/RedTank.png");
	tank->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });
	auto rb = tank->AddComponent<dae::RigidbodyComponent>();

	auto collider = tank->AddComponent<dae::BoxColliderComponent>();
	collider->Initialize(6, 6, 20, 20, false, "Player", { "Walls" });
	collider->AddObserver(rb);


	auto gun = std::make_shared<dae::GameObject>();
	m_pScene->Add(gun);

	gun->Initialize("RedTank_Gun", m_pScene);

	gun->renderer()->SetTexture("Sprites/RedTankGun.png");
	gun->SetParent(tank.get());

	auto tankTextureDim = tank->renderer()->GetTextureDimensions();
	gun->transform()->SetLocalPosition({ -tankTextureDim.x / 2, -tankTextureDim.y / 2 });

	auto gunComponent = gun->AddComponent<dae::GunComponent>();
	gunComponent->Initialize(150.f, 1.5f);

	//TODO change depending on player count
	std::vector<unsigned int> ShootInput
	{
		Controller::GamepadInput::A
	};

	std::vector<unsigned int> MoveInput
	{
		Controller::GamepadInput::LEFT_THUMB
	};

	std::vector<unsigned int> RotationInput
	{
		Controller::GamepadInput::RIGHT_THUMB
	};

	auto player1_MoveCommand{ std::make_unique<dae::GridMoveCommand>(tank.get(), 50.f) };
	auto player1_ShootCommand{ std::make_unique<dae::ShootCommand>(gun.get()) };
	auto player1_GunRotationCommand{ std::make_unique<dae::RotationCommand>(gun.get(), 50.f) };

	InputManager::GetInstance().BindCommand(ShootInput, InputManager::InputType::OnButtonDown, std::move(player1_ShootCommand), 0);
	InputManager::GetInstance().BindCommand(MoveInput, InputManager::InputType::OnAnalog, std::move(player1_MoveCommand), 0);
	InputManager::GetInstance().BindCommand(RotationInput, InputManager::InputType::OnAnalog, std::move(player1_GunRotationCommand), 0);

}

void dae::MapGeneratorComponent::CreateTeleporter(int row, int coll)
{
	auto teleporter = std::make_shared<dae::GameObject>();
	m_pScene->Add(teleporter);
	
	teleporter->Initialize("Teleporter", m_pScene);
	teleporter->renderer()->SetTexture("Sprites/Teleporter.png");
	teleporter->SetParent(GetOwner());
	
	teleporter->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });
}