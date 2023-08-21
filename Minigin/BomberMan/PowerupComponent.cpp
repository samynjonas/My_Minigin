#include "PowerupComponent.h"

#include "GameObject.h"
#include "Scene.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "BoxColliderComponent.h"
#include "BombermanBehaviourComponent.h"
#include "CollisionManager.h"

#include "NumbersTracker.h"
#include "GameListener.h"

#include "sound_system.h"
#include "servicelocator.h"

dae::PowerupComponent::PowerupComponent()
{

}

void dae::PowerupComponent::Initialize(std::shared_ptr<Powerup> powerupToApply)
{
	m_pPowerup = powerupToApply;
}

void dae::PowerupComponent::Update()
{

}

void dae::PowerupComponent::Notify(Event currEvent, Subject* actor)
{
	if (currEvent == TriggerEnter)
	{
		BoxColliderComponent* collider = static_cast<BoxColliderComponent*>(actor);
		if (typeid(*collider) == typeid(BoxColliderComponent)) //TODO improve this
		{
			auto gameObject = collider->GetOtherCollider()->GetOwner();
			if (gameObject != nullptr)
			{
				if (m_pPowerup->ApplyPowerup(gameObject))
				{
					CollisionManager::GetInstance().UnregisterCollider(collider);
					m_pPowerup = std::make_shared<dae::NULLPowerup>();
					m_pPowerup->SpawnPowerup(GetOwner());
				}				
			}
		}
	}
}

//-------------------------------------------- DoorPowerup ---------------------------------
void dae::DoorPowerup::SpawnPowerup(GameObject* powerupGB)
{
	auto renderer = powerupGB->AddComponent<RenderComponent>();
	renderer->Initialize(2);
	renderer->SetTexture("Sprites/Bomberman/Bomberman_Door.png");

	auto trigger = powerupGB->AddComponent<BoxColliderComponent>();
	trigger->Initialize(true, true, "Powerup", { "Player" });

	auto powerupComponent = powerupGB->GetComponent<PowerupComponent>();
	if (powerupComponent)
	{
		trigger->AddObserver(powerupComponent, { TriggerEnter });
	}
}
bool dae::DoorPowerup::ApplyPowerup(GameObject*)
{
	if (NumbersTracker::GetInstance().GetNumber("Enemy") <= 0)
	{
		auto& ss = servicelocator<sound_system>::get_serviceLocator();
		ss.play("Level_End");

		GameListener::GetInstance().SetHasSuccesFullyOpenedDoor(true);
		return true;
	}
	return false;
}

//-------------------------------------------- ExtraBombPowerup ---------------------------------
void dae::ExtraBombPowerup::SpawnPowerup(GameObject* powerupGB)
{
	auto renderer = powerupGB->AddComponent<RenderComponent>();
	renderer->Initialize(2);
	renderer->SetTexture("Sprites/Bomberman/Bomberman_BombPowerup.png"); //TODO Render order

	auto trigger = powerupGB->AddComponent<BoxColliderComponent>();
	trigger->Initialize(true, true, "Powerup", { "Player" });

	auto powerupComponent = powerupGB->GetComponent<PowerupComponent>();
	if (powerupComponent)
	{
		trigger->AddObserver(powerupComponent, { TriggerEnter });
	}

}
bool dae::ExtraBombPowerup::ApplyPowerup(GameObject* player)
{
	auto comp = player->GetComponent<BombermanBehaviourComponent>();
	if (comp)
	{
		auto& ss = servicelocator<sound_system>::get_serviceLocator();
		ss.play("Powerup");

		comp->AddMaxBombs(1);
		return true;
	}
	return false;
}

//-------------------------------------------- DetonatorPowerup ---------------------------------
void dae::DetonatorPowerup::SpawnPowerup(GameObject* powerupGB)
{
	auto renderer = powerupGB->AddComponent<RenderComponent>();
	renderer->Initialize(2);
	renderer->SetTexture("Sprites/Bomberman/Bomberman_DetonatorPowerup.png");

	auto trigger = powerupGB->AddComponent<BoxColliderComponent>();
	trigger->Initialize(true, true, "Powerup", { "Player" });

	auto powerupComponent = powerupGB->GetComponent<PowerupComponent>();
	if (powerupComponent)
	{

		trigger->AddObserver(powerupComponent, { TriggerEnter });
	}
}
bool dae::DetonatorPowerup::ApplyPowerup(GameObject* player)
{
	auto comp = player->GetComponent<BombermanBehaviourComponent>();
	if (comp)
	{
		auto& ss = servicelocator<sound_system>::get_serviceLocator();
		ss.play("Powerup");

		comp->GiveDetonator(true);
		return true;
	}
	return false;
}

//-------------------------------------------- FlamesPowerup ---------------------------------
void dae::FlamesPowerup::SpawnPowerup(GameObject* powerupGB)
{
	auto renderer = powerupGB->AddComponent<RenderComponent>();
	renderer->Initialize(2);
	renderer->SetTexture("Sprites/Bomberman/Bomberman_FlamesPowerup.png");

	auto trigger = powerupGB->AddComponent<BoxColliderComponent>();
	trigger->Initialize(true, true, "Powerup", { "Player" });

	auto powerupComponent = powerupGB->GetComponent<PowerupComponent>();
	if (powerupComponent)
	{

		trigger->AddObserver(powerupComponent, { TriggerEnter });
	}
}
bool dae::FlamesPowerup::ApplyPowerup(GameObject* player)
{
	auto comp = player->GetComponent<BombermanBehaviourComponent>();
	if (comp)
	{
		auto& ss = servicelocator<sound_system>::get_serviceLocator();
		ss.play("Powerup");

		comp->AddExplosionRange(true);
		return true;
	}
	return false;
}

//-------------------------------------------- NULLPowerup ---------------------------------
void dae::NULLPowerup::SpawnPowerup(GameObject* powerupGB)
{
	auto renderer = powerupGB->GetComponent<RenderComponent>();
	if (renderer == nullptr)
	{
		renderer = powerupGB->AddComponent<RenderComponent>();
	}

	renderer->Initialize(1);
	renderer->SetTexture("Sprites/Bomberman/Floor_Sprite.png");
}
bool dae::NULLPowerup::ApplyPowerup(GameObject*)
{
	return false;
}