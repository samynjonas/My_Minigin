#include "MapGeneratorComponent.h"

#include "GameObject.h"
#include "ResourceManager.h"

#include "Scene.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"
#include "HealthComponent.h"
#include "BreakableBehaviourComponent.h"
#include "PowerupComponent.h"

#include "glm/glm.hpp"

#include <string>

dae::MapGeneratorComponent::MapGeneratorComponent()
	: Component()
{

}

void dae::MapGeneratorComponent::Initialize(const std::string& jsonMapFile, int block_size)
{
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
	//return;

	auto wall = std::make_unique<dae::GameObject>();
	auto pWall = wall.get();

	m_pScene->Add(std::move(wall));

	pWall->Initialize("Wall", m_pScene);
	auto renderer = pWall->AddComponent<RenderComponent>();
	if (renderer)
	{
		renderer->Initialize(2);
		renderer->SetTexture("Sprites/Bomberman/Wall_Sprite.png");
	}

	pWall->SetParent(GetOwner());

	pWall->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });
	
	auto collider = pWall->AddComponent<dae::BoxColliderComponent>();
	collider->Initialize(static_cast<int>(m_BlockSize * 0.75f), static_cast<int>(m_BlockSize * 0.75f), false, true, "Walls");
}

void dae::MapGeneratorComponent::CreateFloor(int row, int coll)
{
	int breakableChance{ 4 };

	int randomChance = rand() % breakableChance; //Change for a floor to be a breakable wall
	if (randomChance == 1)
	{
		CreateBreakableWall(row, coll);
		return;
	}

	auto floor = std::make_unique<dae::GameObject>();
	auto pFloor = floor.get();

	m_pScene->Add(std::move(floor));

	pFloor->Initialize("Floor", m_pScene);
	auto renderer = pFloor->AddComponent<RenderComponent>();
	if (renderer)
	{
		renderer->Initialize(1);
		renderer->SetTexture("Sprites/Bomberman/Floor_Sprite.png");
	}
	pFloor->SetParent(GetOwner());

	pFloor->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });
}

void dae::MapGeneratorComponent::CreateBreakableWall(int row, int coll)
{
	auto wall = std::make_unique<dae::GameObject>();
	auto pWall = wall.get();
	m_pScene->Add(std::move(wall));

	pWall->Initialize("BreakableWall", m_pScene);
	pWall->SetParent(GetOwner());

	auto renderer = pWall->AddComponent<RenderComponent>();
	if (renderer)
	{
		renderer->Initialize(2);
		renderer->SetTexture("Sprites/Bomberman/Breakable_Sprite.png");
	}

	pWall->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });
	m_VecBreakablePositions.push_back(pWall->transform()->GetWorldPosition()); //Saving position for adding powerups at end of cycle	
	
	auto behaviour = pWall->AddComponent<BreakableBehaviourComponent>();

	//Powerup Spawning
	behaviour->Initialize(AddPowerup());

	auto health = pWall->AddComponent<HealthComponent>();
	health->Initialize(1, 0);
	
	//TODO When a wall has multiple colliders - do they all get removed?
	auto collider = pWall->AddComponent<dae::BoxColliderComponent>();
	collider->Initialize(static_cast<int>(m_BlockSize * 0.75f), static_cast<int>(m_BlockSize * 0.75f), false, true, "Breakable", { "Explosion" });
	
	collider->AddObserver(behaviour, { CollisionEnter });
	collider->AddObserver(health, { CollisionEnter });
}

std::shared_ptr<dae::Powerup> dae::MapGeneratorComponent::AddPowerup()
{
	int randomChance = rand() % (POWERUP_COUNT + 10);

	if (randomChance >= POWERUP_COUNT - 1)
	{
		return std::make_shared<NULLPowerup>();
	}

	if (randomChance == 0)
	{
		if (m_DoorCount >= MAX_DOORS)
		{
			return AddPowerup();
		}

		m_DoorCount++;
		return std::make_shared<DoorPowerup>();
	}
	else if (randomChance == 1)
	{
		if (m_DoorCount >= MAX_DETONATORS)
		{
			return AddPowerup();
		}

		m_DetonatorCount++;
		return std::make_shared<DetonatorPowerup>();
	}
	else if (randomChance == 2)
	{
		if (m_DoorCount >= MAX_BOMB_POWERUPS)
		{
			return AddPowerup();
		}

		m_BombPowerupCount++;
		return std::make_shared<ExtraBombPowerup>();
	}
	else if (randomChance == 3)
	{
		if (m_DoorCount >= MAX_EXPLOSIONRANGE_POWERUPS)
		{
			return AddPowerup();
		}

		m_ExplosionPowerupCount++;
		return std::make_shared<FlamesPowerup>();
	}

	return std::make_shared<NULLPowerup>();
}

void dae::MapGeneratorComponent::LoadMap()
{
	auto parsedVector = m_CSVParser.ParseCSV(m_JsonMapFile);

	m_Height = (static_cast<int>(parsedVector.size()) - 1) * static_cast<int>(m_BlockSize);
	m_Width	 = (static_cast<int>(parsedVector[0].size()) - 1) * static_cast<int>(m_BlockSize);

	for (int row{}; row < static_cast<int>(parsedVector.size()); ++row)
	{
		for (int coll{}; coll < static_cast<int>(parsedVector[row].size()); ++coll)
		{
			m_WallCount++;
			//Depending on the float value
			switch (static_cast<MapPiece>(parsedVector[row][coll]))
			{
			case MapPiece::Wall:
				CreateWall(row, coll);
				break;
			case MapPiece::Floor:
				CreateFloor(row, coll);
				break;
			default:
				break;
			}
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
}

void dae::MapGeneratorComponent::Notify(Event, Subject*)
{
	//if (currEvent == LevelLoad)
	//{
	//	LoadMap();
	//}
	//else if (currEvent == LevelUnload)
	//{
	//	UnloadMap();
	//}
}

