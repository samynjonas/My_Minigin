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

void dae::BoxColliderComponent::Initialize(int x, int y, int width, int height, bool isTrigger, bool isStatic, std::string layer, std::vector<std::string> collideLayers, std::vector<std::string> skipLayer)
{
	auto& parentPos = GetOwner()->transform()->GetWorldPosition();
	m_PositionOffset = glm::vec2(x, y);

	m_pColliderRect = std::make_unique<Rect>(static_cast<int>(parentPos.x) + x, static_cast<int>(parentPos.y) + y, width, height);

	m_Layer = layer;

	CollisionManager::GetInstance().RegisterCollider(this, layer, collideLayers, skipLayer);
	m_IsStatic = isStatic;
	m_IsTrigger = isTrigger;
}
void dae::BoxColliderComponent::Initialize(int width, int height, bool isTrigger, bool isStatic, std::string layer, std::vector<std::string> collideLayers, std::vector<std::string> skipLayer)
{
	Initialize(0, 0, width, height, isTrigger, isStatic, layer, collideLayers, skipLayer);
}
void dae::BoxColliderComponent::Initialize(bool isTrigger, bool isStatic, std::string layer, std::vector<std::string> collideLayers, std::vector<std::string> skipLayer)
{
	auto textureDim = GetOwner()->GetComponent<RenderComponent>()->GetTextureDimensions();
	Initialize(0, 0, static_cast<int>(textureDim.x), static_cast<int>(textureDim.y), isTrigger, isStatic, layer, collideLayers, skipLayer);
}

bool dae::BoxColliderComponent::IsOverlapping(const Rect* other, BoxColliderComponent* otherCollider)
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

	estimateCollisionPoint(other);
	SetOverlapping(true, m_CollisionPoint);

	m_pOtherCollider = otherCollider;

	return true;
}

void dae::BoxColliderComponent::estimateCollisionPoint(const Rect* other)
{
	int overlapX = std::min(m_pColliderRect.get()->maxX(), other->maxX()) - std::max(m_pColliderRect.get()->minX(), other->minX());
	int overlapY = std::min(m_pColliderRect.get()->maxY(), other->maxY()) - std::max(m_pColliderRect.get()->minY(), other->minY());

	glm::vec2 collisionPoint{};

	collisionPoint.x = std::max(m_pColliderRect.get()->minX(), other->minX()) + overlapX / 2.0f;
	collisionPoint.y = std::max(m_pColliderRect.get()->minY(), other->minY()) + overlapY / 2.0f;

	m_CollisionPoint = collisionPoint;
}

void dae::BoxColliderComponent::SetOverlapping(bool isOverlapping, glm::vec2 collisionPoint)
{
	m_IsOverlapping = isOverlapping;
	m_CollisionPoint = collisionPoint;

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
			if (m_IsTrigger)
			{
				OnTriggerEnter();
			}
			else
			{
				OnCollisionEnter();
			}
		}
		else if(m_IsOverlapping == false && m_WasOverlapping)
		{
			if (m_IsTrigger)
			{
				OnTriggerEnter();
			}
			else
			{
				OnCollisionExit();
			}
		}
		m_WasOverlapping = m_IsOverlapping;
		m_IsDirty = false;
	}
}

void dae::BoxColliderComponent::UpdateTransform()
{
	m_pColliderRect.get()->_x		= static_cast<int>(GetOwner()->transform()->GetWorldPosition().x + m_PositionOffset.x);
	m_pColliderRect.get()->_y		= static_cast<int>(GetOwner()->transform()->GetWorldPosition().y + m_PositionOffset.x);

	CollisionManager::GetInstance().SetDirty();
}

void dae::BoxColliderComponent::PutToSleep()
{
	m_IsSleeping = true;
}

void dae::BoxColliderComponent::WakeUp()
{
	m_IsSleeping = false;
}