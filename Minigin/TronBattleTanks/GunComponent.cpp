#include "GunComponent.h"
#include <iostream>

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"

#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"

#include "Scene.h"

dae::GunComponent::GunComponent()
	: Component()
{

}

dae::GunComponent::~GunComponent()
{

}

void dae::GunComponent::Update()
{

}

void dae::GunComponent::Fire()
{
	//Spawn in a bullet at players position -- should later be changed to gun barrel
	Bullet
	(
		GetOwner()->transform()->GetWorldPosition().x + GetOwner()->renderer()->GetTextureDimensions().x / 2,
		GetOwner()->transform()->GetWorldPosition().y + GetOwner()->renderer()->GetTextureDimensions().y / 2
	);
}

std::shared_ptr<dae::GameObject> dae::GunComponent::Bullet(float x, float y)
{
	std::shared_ptr<dae::GameObject> pBullet = std::make_shared<dae::GameObject>();

	auto pParent = GetOwner();
	auto scene = pParent->GetScene();

	pBullet->Initialize("Bullet", scene);
	pBullet->renderer()->SetTexture("Sprites/BulletPlayer.png");
	pBullet->transform()->SetLocalPosition({ x, y });

	auto rb = pBullet->AddComponent<RigidbodyComponent>();
	rb->ApplyForce({ 150.f, 0.f }, RigidbodyComponent::ForceMode::Force);

	auto collider = pBullet->AddComponent<BoxColliderComponent>();
	collider->Initialize(static_cast<int>(x), static_cast<int>(y), 50, 50);

	scene->Add(pBullet);

	return pBullet;
}