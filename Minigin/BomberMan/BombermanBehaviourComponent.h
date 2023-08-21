#pragma once
#include "Component.h"

#include <memory>
#include <vector>

namespace dae
{
	class Bomb_Component;
	class GameObject;

	class BombermanBehaviourComponent final : public Component
	{
	public:
		BombermanBehaviourComponent();
		~BombermanBehaviourComponent();

		BombermanBehaviourComponent(const BombermanBehaviourComponent& other) = delete;
		BombermanBehaviourComponent(BombermanBehaviourComponent&& other) = delete;
		BombermanBehaviourComponent& operator=(const BombermanBehaviourComponent& other) = delete;
		BombermanBehaviourComponent& operator=(BombermanBehaviourComponent&& other) = delete;

		void Initialize(int gridBlockSize);

		void Update() override;
		void PlaceBomb();
		void Detonate();

		void AddMaxBombs(int amount);
		void AddExplosionRange(int amount);
		void GiveDetonator(bool activate);

		int GetBombsInInventoryCount() const
		{
			return (m_MaxBombs - static_cast<int>(m_vecBombs.size()));
		}

	private:
		//Remember oldest bomb
		//Take care of bomb placing and giving it the level
		void Bomb(float x, float y);

		std::vector<Bomb_Component*> m_vecBombs;

		int m_GridBlockSize{};

		int m_ExplosionRange{ 1 };
		const int MIN_EXPLOSIONRANGE{ 1 };

		int m_MaxBombs{ 1 };
		const int MIN_BOMBS{ 1 };

		bool m_HasDetonator{ false };

		void CleanupBombs();

	};
}

