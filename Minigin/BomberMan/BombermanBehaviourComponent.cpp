#include "BombermanBehaviourComponent.h"

#include "Scene.h"
#include "GameObject.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Bomb_Component.h"
#include "BoxColliderComponent.h"

#include <iostream>

dae::BombermanBehaviourComponent::BombermanBehaviourComponent()
	: Component()
{

}

dae::BombermanBehaviourComponent::~BombermanBehaviourComponent()
{

}

void dae::BombermanBehaviourComponent::Initialize(int gridBlockSize)
{
	m_GridBlockSize = gridBlockSize;
}

void dae::BombermanBehaviourComponent::Update()
{

}

void dae::BombermanBehaviourComponent::AddMaxBombs(int amount)
{
	m_MaxBombs += amount;
	if (m_MaxBombs < MIN_BOMBS)
	{
		m_MaxBombs = MIN_BOMBS;
	}
}

void dae::BombermanBehaviourComponent::AddExplosionRange(int amount)
{
	m_ExplosionRange += amount;
	if (m_ExplosionRange < MIN_EXPLOSIONRANGE)
	{
		m_ExplosionRange = MIN_EXPLOSIONRANGE;
	}
}

void dae::BombermanBehaviourComponent::GiveDetonator(bool activate)
{
	m_HasDetonator = activate;
}

void dae::BombermanBehaviourComponent::PlaceBomb()
{
	CleanupBombs();

	if (static_cast<int>(m_vecBombs.size()) >= m_MaxBombs)
	{
		return;
	}

	Bomb
	(
		GetOwner()->transform()->GetWorldPosition().x + m_GridBlockSize / 2,
		GetOwner()->transform()->GetWorldPosition().y + m_GridBlockSize / 2
	);
}

void dae::BombermanBehaviourComponent::Detonate()
{
	if (m_HasDetonator == false)
	{
		return;
	}

	CleanupBombs();

	if (m_vecBombs.empty())
	{
		return;
	}

	m_vecBombs[0]->MarkForExplode();
}

void dae::BombermanBehaviourComponent::CleanupBombs()
{
	if (m_vecBombs.empty())
	{
		return;
	}

	std::vector<Bomb_Component*> vecBombs;

	for (const auto& pBombs : m_vecBombs)
	{
		if (pBombs == nullptr)
		{
			continue;
		}

		if (pBombs->GetOwner() == nullptr)
		{
			continue;
		}

		if (pBombs->HasExploded())
		{
			continue;
		}

		vecBombs.push_back(pBombs);
	}

	m_vecBombs.clear();
	m_vecBombs = vecBombs;
}

void dae::BombermanBehaviourComponent::Bomb(float x, float y)
{
	auto bomb = std::make_unique<dae::GameObject>();
	auto pBomb = bomb.get();

	auto pParent = GetOwner();
	auto scene = pParent->GetScene();

	pBomb->Initialize("Bomb", scene);
	scene->Add(std::move(bomb));

	auto pRenderer = pBomb->AddComponent<RenderComponent>();
	pRenderer->Initialize(2);
	pRenderer->SetTexture("Sprites/Bomberman/Bomberman_Bomb.png");

	int grid_X = static_cast<int>(x / m_GridBlockSize) * m_GridBlockSize;
	int grid_Y = static_cast<int>(y / m_GridBlockSize) * m_GridBlockSize;

	pBomb->transform()->SetLocalPosition({ grid_X, grid_Y });

	auto pBombComp = pBomb->AddComponent<Bomb_Component>();
	pBombComp->Initialize(m_GridBlockSize, m_ExplosionRange);

	auto collider = pBomb->AddComponent<BoxColliderComponent>();
	collider->Initialize(m_GridBlockSize, m_GridBlockSize, false, true, "Bombs");

	//Adding bombs to keep track of oldest
	m_vecBombs.push_back(pBombComp);
}
