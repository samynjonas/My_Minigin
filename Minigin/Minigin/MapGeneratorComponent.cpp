#include "MapGeneratorComponent.h"

#include "GameObject.h"
#include "ResourceManager.h"

#include "Scene.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"

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
	auto wall = std::make_unique<dae::GameObject>();
	m_pScene->Add(std::move(wall));
	
	wall->Initialize("Wall", m_pScene);
	wall->SetParent(GetOwner());
	
	auto renderer = wall->AddComponent<RenderComponent>();
	if (renderer)
	{
		renderer->Initialize(2);
		renderer->SetTexture("Sprites/Bomberman/Wall_Sprite.png")
	}

	wall->transform()->SetLocalPosition( { m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize} );
	auto collider = wall->AddComponent<dae::BoxColliderComponent>();
	collider->Initialize(static_cast<int>(m_BlockSize), static_cast<int>(m_BlockSize), false, true, "Walls");
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
	m_pScene->Add(std::move(floor));
	
	floor->Initialize("Floor", m_pScene);
	auto renderer = floor->AddComponent<RenderComponent>();
	if (renderer)
	{
		renderer->Initialize(1);
		renderer->SetTexture("Sprites/Bomberman/Floor_Sprite.png")
	}


	floor->SetParent(GetOwner());
	
	floor->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });
}

void dae::MapGeneratorComponent::CreateBreakableWall(int row, int coll)
{
	auto wall = std::make_unique<dae::GameObject>();
	m_pScene->Add(std::move(wall));

	wall->Initialize("Wall", m_pScene);
	wall->SetParent(GetOwner());

	auto renderer = wall->AddComponent<RenderComponent>();
	if (renderer)
	{
		renderer->Initialize(2);
		renderer->SetTexture("Sprites/Bomberman/Breakable_Sprite.png");
	}

	wall->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });
	auto collider = wall->AddComponent<dae::BoxColliderComponent>();
	collider->Initialize(static_cast<int>(m_BlockSize), static_cast<int>(m_BlockSize), false, true, "Walls");

}

void dae::MapGeneratorComponent::LoadMap()
{
	auto parsedVector = m_CSVParser.ParseCSV(m_JsonMapFile);

	m_Height = (static_cast<int>(parsedVector.size()) - 1) * static_cast<int>(m_BlockSize);
	m_Width = (static_cast<int>(parsedVector[0].size()) - 1) * static_cast<int>(m_BlockSize);

	for (int row{}; row < static_cast<int>(parsedVector.size()); ++row)
	{
		for (int coll{}; coll < static_cast<int>(parsedVector[row].size()); ++coll)
		{
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

void dae::MapGeneratorComponent::Notify(Event currEvent, Subject*)
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

