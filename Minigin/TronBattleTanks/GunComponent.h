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

		void Update() override;

		void Fire();

	private:
		std::shared_ptr<dae::GameObject> Bullet(float x, float y);
	};
}


