#pragma once
#include "Component.h"
#include <memory>

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

		void Initialize(float shootForce, float cooldown);
		void Update() override;

		void Fire();

	private:
		std::shared_ptr<dae::GameObject> Bullet(float x, float y);

		float m_ShootForce{ 150.f };
		float m_ShootCooldown{ 0.5f };

		float m_ElapsedTime{ 0.f };

		bool m_HasShot{ false };

	};
}


