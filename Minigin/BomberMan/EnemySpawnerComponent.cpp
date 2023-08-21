#include "EnemySpawnerComponent.h"

#include <memory>

#include "Scene.h"
#include "GameObject.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "AIBehaviourComponent.h"
#include "BoxColliderComponent.h"
#include "HealthComponent.h"

#include "NumbersTracker.h"


dae::EnemySpawnerComponent::EnemySpawnerComponent()
	: Component()
{

}

void dae::EnemySpawnerComponent::Initialize(int count, int points, int speed, int smart, std::string texturePath, glm::vec4 spawnRect, int gridSize, bool spawnOnInit)
{
	m_EnemyCounter = count;

	m_SpawnRect = spawnRect;
	m_GridSize = gridSize;

	m_Points = points;
	m_Speed = speed;
	m_Smart = smart;
	m_TexturePath = texturePath;

	if (spawnOnInit)
	{
		Spawn();
	}
}

void dae::EnemySpawnerComponent::Spawn()
{
	NumbersTracker::GetInstance().AddToTracker("Enemy", m_EnemyCounter);

	for (int index = 0; index < m_EnemyCounter; index++)
	{
		InitEnemy();
	}
}

void dae::EnemySpawnerComponent::InitEnemy()
{
	auto pScene = GetOwner()->GetScene();

	auto Enemy = std::make_unique<dae::GameObject>();
	{
		auto pEnemy = Enemy.get();

		pScene->Add(std::move(Enemy));

		pEnemy->Initialize("Enemy", pScene);

		auto renderComp = pEnemy->AddComponent<RenderComponent>();
		if (renderComp)
		{
			renderComp->Initialize(2);
			renderComp->SetTexture(m_TexturePath);
		}

		pEnemy->transform()->SetLocalPosition({ 128.f, 160.f });

		auto rb = pEnemy->AddComponent<RigidbodyComponent>();

		auto aiBehaviour = pEnemy->AddComponent<AIBehaviourComponent>();
		aiBehaviour->Initialize(10.f * m_Speed, m_Smart);

		//TODO improve random position spawning
		int randomX = rand() % static_cast<int>(m_SpawnRect.z) + static_cast<int>(m_SpawnRect.x);
		int randomY = rand() % static_cast<int>(m_SpawnRect.w) + static_cast<int>(m_SpawnRect.y);

		randomX = static_cast<int>(randomX / m_GridSize) * randomX;
		randomY = static_cast<int>(randomY / m_GridSize) * randomY;

		auto collider = pEnemy->AddComponent<BoxColliderComponent>();
		collider->Initialize(6, 6, randomX, randomY, false, false, "Enemy", { "Walls", "Breakable", "Bombs"});

		collider->AddObserver(aiBehaviour, { CollisionEnter });
		collider->AddObserver(rb, { CollisionEnter, CollisionExit });

		auto health = pEnemy->AddComponent<HealthComponent>();
		health->Initialize(1, 0);
		health->AddObserver(this, { ObjectDied } );

		auto trigger = pEnemy->AddComponent<BoxColliderComponent>();
		trigger->Initialize(true, false, "Enemy", { "Explosion" });

		trigger->AddObserver(health, { TriggerEnter });
	}
}

void dae::EnemySpawnerComponent::Notify(Event currEvent, Subject*)
{
	if (currEvent == ObjectDied)
	{
		m_EnemyCounter--;
		NumbersTracker::GetInstance().AddToTracker("Enemy", -1);
		NumbersTracker::GetInstance().AddToTracker("Score", m_Points);

	}

	if (m_EnemyCounter <= 0)
	{
		std::cout << "All Enemies Died" << std::endl;
	}
}