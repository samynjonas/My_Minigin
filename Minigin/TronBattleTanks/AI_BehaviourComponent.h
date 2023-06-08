#pragma once
#include "Component.h"

namespace dae
{
	class AI_BehaviourComponent final : public Component
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

	private:
		float m_MoveSpeed{};


	};
}

