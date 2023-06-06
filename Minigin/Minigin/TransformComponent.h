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

		void Update(float) override {};

		//Translation
		const glm::vec2& GetLocalPosition() const;
		const glm::vec2& GetWorldPosition();

		void SetLocalPosition(const glm::vec2& position); //Sets the local position
		void SetWorldPosition(const glm::vec2& position); //Sets the world position

		void Translate(const glm::vec2& translation, bool isLocal = false); //Adds to position

		//Rotation
		const float& GetLocalRotation() const;
		const float& GetWorldRotation();

		void SetLocalRotation(const float& degrees); //Sets the local rotation
		void SetWorldRotation(const float& degrees); //Sets the world rotation

		void Rotate(const float& degrees, bool isLocal = false); //Adds to rotation

		void UpdateTranslation();
		void UpdateRotation();
	private:

		glm::vec2 m_WorldPosition{ 0.f, 0.f};
		glm::vec2 m_LocalPosition{};
		
		float m_WorldRotation{ 0.f };
		float m_LocalRotation{ 0.f };

		bool m_IsPositionDirty{ false };
		bool m_IsRotationDirty{ false };

	};

}

