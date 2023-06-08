#include "GunComponent.h"
#include <iostream>

#include "GameObject.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"
#include "HealthComponent.h"

#include "Scene.h"
#include "MiniginTimer.h"
#include "sound_system.h"
#include "servicelocator.h"

dae::GunComponent::GunComponent()
	: Component()
{

}

dae::GunComponent::~GunComponent()
{

}

void dae::GunComponent::Initialize(float shootForce = 150.f, float cooldown = 0.5f)
{
	m_ShootForce = shootForce;
	m_ShootCooldown = cooldown;
}

void dae::GunComponent::Update()
{
	if (m_HasShot)
	{
		if (m_ElapsedTime >= m_ShootCooldown)
		{
			m_ElapsedTime = 0;

			m_HasShot = false;
		}

		m_ElapsedTime += MiniginTimer::GetInstance().GetDeltaTime();
	}
}

void dae::GunComponent::Fire()
{
	if (m_HasShot)
	{
		return;
	}

	//Spawn in a bullet at players position -- should later be changed to gun barrel
	Bullet
	(
		GetOwner()->transform()->GetWorldPosition().x + GetOwner()->renderer()->GetTextureDimensions().x / 2,
		GetOwner()->transform()->GetWorldPosition().y + GetOwner()->renderer()->GetTextureDimensions().y / 2
	);


	//Playing shoot sound
	auto& ss = servicelocator<sound_system>::get_serviceLocator();
	ss.play(0, 100);

	m_HasShot = true;
}

std::shared_ptr<dae::GameObject> dae::GunComponent::Bullet(float x, float y)
{
	std::shared_ptr<dae::GameObject> pBullet = std::make_shared<dae::GameObject>();

	auto pParent = GetOwner();
	auto scene = pParent->GetScene();

	pBullet->Initialize("Bullet", scene);
	scene->Add(pBullet);

	pBullet->renderer()->SetTexture("Sprites/BulletPlayer.png");
	pBullet->transform()->SetLocalPosition({ x, y });

	auto rb = pBullet->AddComponent<RigidbodyComponent>();

	const auto& gunAngle = pParent->transform()->GetWorldRotation() * ( 3.14f / 180.f ); //TODO improve PI

	float xDirection = std::cosf(gunAngle);
	float yDirection = std::sinf(gunAngle);

	glm::vec2 normalizedDirection{ xDirection, yDirection };
	normalizedDirection *= m_ShootForce;

	rb->ApplyForce(normalizedDirection, RigidbodyComponent::ForceMode::Force);
	rb->PhysicsMaterial().bounciness = 1;

	auto collider = pBullet->AddComponent<BoxColliderComponent>();
	collider->Initialize(false, { "Environment", "Enemy" }, { "Player" });
	collider->AddObserver(rb);

	pBullet->AddComponent<HealthComponent>();

	return pBullet;
}