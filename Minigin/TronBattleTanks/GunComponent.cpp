#include "GunComponent.h"
#include <iostream>

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"


dae::GunComponent::GunComponent()
	: Component()
{

}

dae::GunComponent::~GunComponent()
{

}

void dae::GunComponent::Update(float)
{

}

void dae::GunComponent::Fire()
{
	//Spawn in a bullet at players position -- should later be changed to gun barrel
	Bullet
	(
		GetOwner()->transform()->GetPosition().x,
		GetOwner()->transform()->GetPosition().y
	);
}

std::shared_ptr<dae::GameObject> dae::GunComponent::Bullet(float x, float y)
{
	std::shared_ptr<dae::GameObject> pBullet = std::make_shared<dae::GameObject>();
	pBullet->Initialize("Bullet");
	pBullet->renderer()->SetTexture("Sprites/BulletPlayer.png");
	pBullet->transform()->SetPosition(x, y, 0);

	return pBullet;
}