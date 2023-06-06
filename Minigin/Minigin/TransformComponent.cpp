#include "TransformComponent.h"
#include "GameObject.h"

dae::TransformComponent::TransformComponent()
	: Component()
{

}

dae::TransformComponent::~TransformComponent()
{

}

void dae::TransformComponent::SetLocalPosition(const glm::vec2& position)
{
	m_LocalPosition.x = position.x;
	m_LocalPosition.y = position.y;

	m_IsPositionDirty = true;
}

const glm::vec2& dae::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

void dae::TransformComponent::SetWorldPosition(const glm::vec2& position)
{
	m_WorldPosition.x = position.x;
	m_WorldPosition.y = position.y;

	m_IsPositionDirty = true;
}

const glm::vec2& dae::TransformComponent::GetWorldPosition()
{
	if (m_IsPositionDirty)
	{
		UpdateTranslation();
	}
	return m_WorldPosition;
}

void dae::TransformComponent::Translate(const glm::vec2& position, bool isLocal)
{
	if (isLocal)
	{
		m_LocalPosition.x += position.x;
		m_LocalPosition.y += position.y;
	}
	else
	{
		m_WorldPosition.x += position.x;
		m_WorldPosition.y += position.y;
	}

	m_IsPositionDirty = true;
}

void dae::TransformComponent::SetLocalRotation(const float& degrees)
{
	m_LocalRotation = degrees;

	m_IsRotationDirty = true;
}

const float& dae::TransformComponent::GetLocalRotation() const
{
	return m_LocalRotation;
}

void dae::TransformComponent::SetWorldRotation(const float& degrees)
{
	m_WorldRotation = degrees;

	m_IsRotationDirty = true;
}

const float& dae::TransformComponent::GetWorldRotation()
{
	if (m_IsRotationDirty)
	{
		UpdateRotation();
	}
	return m_WorldRotation;
}

void dae::TransformComponent::Rotate(const float& degrees, bool isLocal)
{
	if (isLocal)
	{
		m_LocalRotation += degrees;
	}
	else
	{
		m_WorldRotation += degrees;
	}
	m_IsRotationDirty = true;
}

void dae::TransformComponent::UpdateTranslation()
{
	m_IsPositionDirty = false;

	if (GetOwner()->GetParent() == nullptr)
	{
		m_WorldPosition = m_LocalPosition;
		//return;
	}
	else
	{
		auto& parentPos = GetOwner()->GetParent()->transform()->GetWorldPosition();
		m_WorldPosition = parentPos + m_LocalPosition;
	}


	for (auto& child : GetOwner()->GetChildren())
	{
		child->transform()->UpdateTranslation();
	}
}

void dae::TransformComponent::UpdateRotation()
{
	m_IsRotationDirty = false;

	if (GetOwner()->GetParent() == nullptr)
	{
		m_WorldRotation = m_LocalRotation;
		return;
	}

	auto& parentRot = GetOwner()->GetParent()->transform()->GetWorldRotation();
	m_WorldRotation = parentRot + m_LocalRotation;

	for (auto& child : GetOwner()->GetChildren())
	{
		child->transform()->UpdateRotation();
	}
}