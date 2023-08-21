#include "BreakableBehaviourComponent.h"

#include "GameObject.h"
#include "Scene.h"

#include "PowerupComponent.h"
#include "TransformComponent.h"

dae::BreakableBehaviourComponent::BreakableBehaviourComponent()
{

}

void dae::BreakableBehaviourComponent::Initialize(std::shared_ptr<Powerup> powerupToApply)
{
	m_pHiddenPowerup = powerupToApply;
}

void dae::BreakableBehaviourComponent::Update()
{

}

void dae::BreakableBehaviourComponent::Notify(Event currEvent, Subject*)
{
	if (currEvent == CollisionEnter)
	{
		//Spawn powerup
		Break();
	}
}

void dae::BreakableBehaviourComponent::Break()
{
	auto powerup = std::make_unique<dae::GameObject>();
	auto pPowerup = powerup.get();

	auto pParent = GetOwner();
	auto scene = pParent->GetScene();

	pPowerup->Initialize("Powerup", scene);
	scene->Add(std::move(powerup));

	pPowerup->transform()->SetLocalPosition({ GetOwner()->transform()->GetWorldPosition() });

	auto powerupComp = pPowerup->AddComponent<PowerupComponent>();
	powerupComp->Initialize(m_pHiddenPowerup);

	//Applying powerup-things
	m_pHiddenPowerup->SpawnPowerup(pPowerup);
}