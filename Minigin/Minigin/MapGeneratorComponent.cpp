#include "MapGeneratorComponent.h"
#include "GameObject.h"
#include "Scene.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"

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
				CreateFloor(row, coll);
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


void dae::MapGeneratorComponent::CreateWall(int row, int coll)
{
	auto wall = std::make_shared<dae::GameObject>();
	m_pScene->Add(wall);
	
	wall->Initialize("Wall", m_pScene);
	wall->renderer()->SetTexture("Sprites/WallPiece.png");
	wall->SetParent(GetOwner());

	wall->transform()->SetLocalPosition( { m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize} );
	auto collider = wall->AddComponent<dae::BoxColliderComponent>();
	collider->Initialize(static_cast<int>(m_BlockSize), static_cast<int>(m_BlockSize), true);
}

void dae::MapGeneratorComponent::CreateFloor(int row, int coll)
{
	auto floor = std::make_shared<dae::GameObject>();
	m_pScene->Add(floor);

	floor->Initialize("Floor", m_pScene);
	floor->renderer()->SetTexture("Sprites/FloorPiece.png");
	floor->SetParent(GetOwner());
	
	floor->transform()->SetLocalPosition({ m_ParentPos.x + coll * m_BlockSize, m_ParentPos.y + row * m_BlockSize });
}