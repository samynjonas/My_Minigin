#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include <vector>

namespace dae
{
	class GunComponent final : public Component
	{
	public:
		GunComponent();
		~GunComponent();

		GunComponent(const GunComponent& other) = delete;
		GunComponent(GunComponent&& other) = delete;
		GunComponent& operator=(const GunComponent& other) = delete;
		GunComponent& operator=(GunComponent&& other) = delete;

		void Initialize(const std::string shooterlayer, const std::vector<std::string> hitLayer, float shootForce = 150.f, float cooldown = 0.5f);
		void Update() override;

		void Fire();
		bool IsInCooldown() const
		{
			return m_HasShot;
		}

	private:
		std::shared_ptr<dae::GameObject> Bullet(float x, float y);

		float m_ShootForce{ 150.f };
		float m_ShootCooldown{ 0.5f };

		float m_ElapsedTime{ 0.f };

		bool m_HasShot{ false };

		std::string m_ShooterLayer{};
		std::vector<std::string> m_HitLayers{};

	};
}


