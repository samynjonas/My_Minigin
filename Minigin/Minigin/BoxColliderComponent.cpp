#include "BoxColliderComponent.h"
#include "CollisionManager.h"
#include "GameObject.h"

#include "RenderComponent.h"
#include "TransformComponent.h"

dae::BoxColliderComponent::BoxColliderComponent()
	: Component()
{

}

dae::BoxColliderComponent::~BoxColliderComponent()
{
	CollisionManager::GetInstance().UnregisterCollider(this);
}

void dae::BoxColliderComponent::Initialize(int x, int y, int width, int height)
{
	m_pColliderRect = std::make_unique<Rect>(x, y, width, height);

	CollisionManager::GetInstance().RegisterCollider(this);

	std::cout << "Registed collider" << std::endl;
}

bool dae::BoxColliderComponent::IsOverlapping(const Rect* other)
{
	UpdateTransform();
	Rect* thisCollider = m_pColliderRect.get();

	if (thisCollider->_x + thisCollider->_width < other->_x)
	{
		return false;
	}

	if (thisCollider->_x > other->_x + other->_width)
	{
		return false;
	}

	//At this point you know X is colliding

	if (thisCollider->_y + thisCollider->_height < other->_y)
	{
		return false;
	}

	if (thisCollider->_y > other->_y + other->_height)
	{
		return false;
	}

	SetOverlapping(true);

	return true;
}

void dae::BoxColliderComponent::SetOverlapping(bool isOverlapping)
{
	m_IsOverlapping = isOverlapping;
}

void dae::BoxColliderComponent::Update()
{
	if (m_IsOverlapping)
	{
		OnCollisionEnter();
		m_IsOverlapping = false;
	}
}

void dae::BoxColliderComponent::UpdateTransform()
{
	m_pColliderRect.get()->_x		= static_cast<int>(GetOwner()->transform()->GetWorldPosition().x);
	m_pColliderRect.get()->_y		= static_cast<int>(GetOwner()->transform()->GetWorldPosition().y);
	m_pColliderRect.get()->_width	= static_cast<int>(GetOwner()->renderer()->GetTextureDimensions().x);
	m_pColliderRect.get()->_height	= static_cast<int>(GetOwner()->renderer()->GetTextureDimensions().y);

}