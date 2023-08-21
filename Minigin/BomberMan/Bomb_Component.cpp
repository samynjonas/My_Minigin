#include "Bomb_Component.h"
#include "MiniginTimer.h"

#include "servicelocator.h"
#include "Console_Logger.h"
#include "sound_system.h"

#include "Scene.h"
#include "GameObject.h"

#include "BoxColliderComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "HealthComponent.h"
#include "CounterComponent.h"

dae::Bomb_Component::Bomb_Component()
	: Component()
{

}

void dae::Bomb_Component::Initialize(int gridSize, int level = 1)
{
	m_GridSize = gridSize;

	//Level will determine the blast radius level = squares in four directions
	m_Level = level;

	//Time a bomb takes to explode
	m_DetonationTime = 3.f;

}

void dae::Bomb_Component::Update()
{
	if (m_HasExploded == false)
	{
		if (m_DetonationTime > 0)
		{
			m_DetonationTime -= MiniginTimer::GetInstance().GetDeltaTime();
		}
		else
		{
			Explode();
		}
	}

	if (m_HasExploded)
	{
		GetOwner()->MarkForDead();
	}
}

void dae::Bomb_Component::Explode()
{
	glm::vec2 BombPosition = GetOwner()->transform()->GetWorldPosition();

	std::string explosionLayer{ "Explosion" };

	auto& ss = servicelocator<sound_system>::get_serviceLocator();
	ss.play("BombExplosion");

	//Detonate in four direction with "level" distance
	for (int i = -m_Level; i < -m_Level + (1 + m_Level * 2); i++)
	{
		SpawnExplosion(BombPosition.x - m_GridSize * i, BombPosition.y);
	}

	for (int i = -m_Level; i < -m_Level + (1 + m_Level * 2); i++)
	{
		SpawnExplosion(BombPosition.x, BombPosition.y - m_GridSize * i);
	}

	m_HasExploded = true;
}

void dae::Bomb_Component::MarkForExplode()
{
	m_DetonationTime = 0;
}

void dae::Bomb_Component::SpawnExplosion(float x, float y)
{
	auto explosion = std::make_unique<dae::GameObject>();
	auto pExplosion = explosion.get();

	auto pParent = GetOwner();
	auto scene = pParent->GetScene();

	pExplosion->Initialize("Explosion", scene);
	scene->Add(std::move(explosion));

	auto pRenderer = pExplosion->AddComponent<RenderComponent>();
	pRenderer->Initialize(2);
	pRenderer->SetTexture("Sprites/Bomberman/Bomberman_Explosion_Single.png");
	
	pExplosion->transform()->SetLocalPosition({ x, y });

	auto BlastCollider = pExplosion->AddComponent<BoxColliderComponent>();
	BlastCollider->Initialize(1, 1, true, false, "Explosion", { "Breakable", "Player", "Enemy" });
	
	auto counter = pExplosion->AddComponent<CounterComponent>();
	counter->Initialize(1.25f);

	auto Health = pExplosion->AddComponent<HealthComponent>();
	Health->Initialize(1, 0);
	
	counter->AddObserver(Health, { CounterFinished });
}