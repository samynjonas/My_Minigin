#pragma once
#include "Component.h"
#include "Observer.h"

#include <memory>

namespace dae
{
	class Powerup;

	class BreakableBehaviourComponent final : public Component, public Observer
	{
	public:
		BreakableBehaviourComponent();
		~BreakableBehaviourComponent() = default;

		BreakableBehaviourComponent(const BreakableBehaviourComponent& other) = delete;
		BreakableBehaviourComponent(BreakableBehaviourComponent&& other) = delete;
		BreakableBehaviourComponent& operator=(const BreakableBehaviourComponent& other) = delete;
		BreakableBehaviourComponent& operator=(BreakableBehaviourComponent&& other) = delete;

		void Initialize(std::shared_ptr<Powerup> powerupToApply);
		void Update() override;

		void Notify(Event currEvent, Subject* actor);

	private:
		void Break();

		std::shared_ptr<Powerup> m_pHiddenPowerup{ nullptr };
	};

	

}

