#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class RigidbodyComponent;
	class AI_BehaviourComponent final : public Component, public Observer
	{
	public:
		AI_BehaviourComponent();
		~AI_BehaviourComponent();

		AI_BehaviourComponent(const AI_BehaviourComponent& other) = delete;
		AI_BehaviourComponent(AI_BehaviourComponent&& other) = delete;
		AI_BehaviourComponent& operator=(const AI_BehaviourComponent& other) = delete;
		AI_BehaviourComponent& operator=(AI_BehaviourComponent&& other) = delete;

		void Initialize(const float& moveSpeed);
		void Update() override;
		void Notify(Event currEvent, subject* actor);

	private:
		void ChangeDirection();

		float m_MoveSpeed{};
		const int MAX_DISTANCE{ 100 };

		float m_ElapsedSec{};
		
		const float ORIGINAL_MOVE_TILL_CHANGE{ 1.5f };
		float move_till_change{ ORIGINAL_MOVE_TILL_CHANGE };

		RigidbodyComponent* m_pRigidbody{ nullptr };
	};
}


