#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class RigidbodyComponent final : public Component
	{
	public:
		RigidbodyComponent();
		~RigidbodyComponent();

		RigidbodyComponent(const RigidbodyComponent& other) = delete;
		RigidbodyComponent(RigidbodyComponent&& other) = delete;
		RigidbodyComponent& operator=(const RigidbodyComponent& other) = delete;
		RigidbodyComponent& operator=(RigidbodyComponent&& other) = delete;

		void Update() override;

		enum class ForceMode
		{
			None,
			Force,
			Impulse
		};

		void ApplyForce(const glm::vec2& vector, ForceMode mode);

		void SetUseGravity(bool state);
		void SetMass(float mass);

	private:
		bool m_UseGravity{ false };
		
		const float GRAVITY{ 9.81f };
		const float DRAG{ 5.f };

		ForceMode m_ActiveForceMode{ ForceMode::None };


		float m_Mass{ 1.f };
		glm::vec2 m_Velocity{ 0.f, 0.f };

	};
}

