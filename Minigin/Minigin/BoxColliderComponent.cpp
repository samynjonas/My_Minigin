#include "BoxColliderComponent.h"

dae::BoxColliderComponent::BoxColliderComponent()
	: Component()
{

}

void dae::BoxColliderComponent::Initialize(int x, int y, int width, int height)
{
	m_pColliderRect = std::make_unique<Rect>(x, y, width, height);
}

void dae::BoxColliderComponent::Update(float)
{

}