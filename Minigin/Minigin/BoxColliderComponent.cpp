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

void dae::BoxColliderComponent::Initialize(int x, int y, int width, int height, bool isStatic)
{
	auto parentPos = GetOwner()->transform()->GetWorldPosition();
	m_PositionOffset = glm::vec2(x, y);

	m_pColliderRect = std::make_unique<Rect>(static_cast<int>(parentPos.x) + x, static_cast<int>(parentPos.y) + y, width, height);

	CollisionManager::GetInstance().RegisterCollider(this);
	m_IsStatic = isStatic;
}

void dae::BoxColliderComponent::Initialize(int width, int height, bool isStatic)
{
	Initialize(0, 0, width, height, isStatic);
}

void dae::BoxColliderComponent::Initialize(bool isStatic)
{
	auto textureDim = GetOwner()->renderer()->GetTextureDimensions();
	Initialize(0, 0, static_cast<int>(textureDim.x), static_cast<int>(textureDim.y), isStatic);
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
	if (m_WasOverlapping != m_IsOverlapping)
	{
		m_IsDirty = true;
	}
}

void dae::BoxColliderComponent::Update()
{
	if (m_IsDirty)
	{
		if (m_IsOverlapping && m_WasOverlapping == false)
		{
			OnCollisionEnter();
		}
		else if(m_IsOverlapping == false && m_WasOverlapping)
		{
			OnCollisionExit();
		}
		m_WasOverlapping = m_IsOverlapping;
		m_IsDirty = false;
	}
}

void dae::BoxColliderComponent::UpdateTransform()
{
	m_pColliderRect.get()->_x		= static_cast<int>(GetOwner()->transform()->GetWorldPosition().x + m_PositionOffset.x);
	m_pColliderRect.get()->_y		= static_cast<int>(GetOwner()->transform()->GetWorldPosition().y + m_PositionOffset.x);

}

void dae::BoxColliderComponent::PutToSleep()
{
	m_IsSleeping = true;
}

void dae::BoxColliderComponent::WakeUp()
{
	m_IsSleeping = false;
}