#pragma once
#include "Component.h"
#include <memory>

namespace dae
{
	class Bomb_Component final : public Component
	{
	public:
		Bomb_Component();
		~Bomb_Component() = default;

		Bomb_Component(const Bomb_Component& other) = delete;
		Bomb_Component(Bomb_Component&& other) = delete;
		Bomb_Component& operator=(const Bomb_Component& other) = delete;
		Bomb_Component& operator=(Bomb_Component&& other) = delete;

		void Initialize(int gridSize, int level);
		void Update() override;

		bool HasExploded()
		{
			return m_HasExploded;
		}

		void MarkForExplode();

	private:
		int m_GridSize{};
		int m_Level{};
		float m_DetonationTime{};

		bool m_HasExploded{ false };

		void Explode();

		void SpawnExplosion(float x, float y);
	};
}