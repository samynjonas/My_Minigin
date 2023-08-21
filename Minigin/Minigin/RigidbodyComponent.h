#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include "Observer.h"

namespace dae
{
	//TODO This isnt necesary
	struct PhysicsMaterial
	{
		float bounciness{ 0.f };
	};

	class RigidbodyComponent final : public Component, public Observer
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
			Force
		};

		void ApplyForce(const glm::vec2& vector, ForceMode mode);

		void SetUseGravity(bool state);
		void SetMass(float mass);

		void SetPhysicsMaterial(const PhysicsMaterial& physicsMaterial);
		PhysicsMaterial& PhysicsMaterial()
		{
			return m_PhysicsMaterial;
		}

		void Notify(Event currEvent, Subject* actor) override;

		glm::vec2 GetDirection() const
		{
			const float vectorLength = static_cast<float>(m_Velocity.length());
			return m_Velocity / vectorLength;
		}

	private:
		bool m_UseGravity{ false };
		bool m_IsColliding{ false };

		const float GRAVITY{ 9.81f };
		const float DRAG{ 5.f };

		dae::PhysicsMaterial m_PhysicsMaterial{};

		ForceMode m_ActiveForceMode{ ForceMode::None };

		float m_Mass{ 1.f };
		glm::vec2 m_Velocity{ 0.f, 0.f };
		glm::vec2 m_CollidingVelocity{ 0.f, 0.f};
	};
}

