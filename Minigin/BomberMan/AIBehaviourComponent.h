#pragma once
#include "Component.h"
#include "Observer.h"
#include "glm/glm.hpp"

namespace dae
{
	class RigidbodyComponent;
	class AIBehaviourComponent final : public Component, public Observer
	{
	public:
		AIBehaviourComponent();
		~AIBehaviourComponent();

		AIBehaviourComponent(const AIBehaviourComponent& other) = delete;
		AIBehaviourComponent(AIBehaviourComponent&& other) = delete;
		AIBehaviourComponent& operator=(const AIBehaviourComponent& other) = delete;
		AIBehaviourComponent& operator=(AIBehaviourComponent&& other) = delete;

		void Initialize(const float& moveSpeed, int smart);
		void Update() override;

		void Notify(Event currEvent, Subject* actor) override;

	private:
		void ChangeDirection();

		int m_Smart{};

		float m_MoveSpeed{};
		const int MAX_DISTANCE{ 96 };

		float m_ElapsedSec{};

		const float ORIGINAL_MOVE_TILL_CHANGE{ 5.f };
		float move_till_change{ ORIGINAL_MOVE_TILL_CHANGE };

		RigidbodyComponent* m_pRigidbody{ nullptr };

	};
}

