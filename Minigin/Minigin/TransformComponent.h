#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent();
		~TransformComponent();

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		void Update(float deltaTime) override;

		const glm::vec3& GetPosition() const
		{
			return m_Position;
		}
		void SetPosition(float x, float y, float z);

		const float& GetRotation() const
		{
			return m_Rotation;
		}
		void SetRotation(float angle);

	private:
		glm::vec3 m_Position{ 0.f, 0.f , 0.f };
		float m_Rotation{ 0.f };

	};

}

