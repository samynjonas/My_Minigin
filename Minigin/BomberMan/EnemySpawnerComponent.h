#pragma once
#include "Component.h"
#include "Observer.h"

#include "glm/glm.hpp"
#include <string>

namespace dae
{
	class EnemySpawnerComponent final : public Component, public Observer
	{
	public:
		EnemySpawnerComponent();
		~EnemySpawnerComponent() = default;

		EnemySpawnerComponent(const EnemySpawnerComponent& other) = delete;
		EnemySpawnerComponent(EnemySpawnerComponent&& other) = delete;
		EnemySpawnerComponent& operator=(const EnemySpawnerComponent& other) = delete;
		EnemySpawnerComponent& operator=(EnemySpawnerComponent&& other) = delete;

		void Initialize(int count, int points, int speed, int smart, std::string texturePath, glm::vec4 spawnRect, int gridSize, bool spawnOnInit = true);
		void Update() override {};
		void Spawn();

		int GetCount() const
		{
			return m_EnemyCounter;
		}

		void Notify(Event currEvent, Subject* actor) override;

	private:
		void InitEnemy();

		int m_EnemyCounter{};

		int m_Points{};
		int m_Speed{};
		int m_Smart{};
		std::string m_TexturePath{};
		
		int m_GridSize{};
		glm::vec4 m_SpawnRect{};

	};	
}


