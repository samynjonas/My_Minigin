#include "CollisionManager.h"
#include "BoxColliderComponent.h"

void dae::CollisionManager::RegisterCollider(BoxColliderComponent* collider)
{
	m_pColliders.push_back(collider);
	m_IsDirty = true;
}

void dae::CollisionManager::UnregisterCollider(BoxColliderComponent* collider)
{
	for (auto itr{ m_pColliders.begin() }; itr != m_pColliders.end(); ++itr)
	{
		if (*itr == collider)
		{
			m_pColliders.erase(itr);
		}
	}

	m_IsDirty = true;
}

void dae::CollisionManager::Update()
{
	if (m_IsDirty)
	{
		for (auto& collider : m_pColliders)
		{
			if (collider->IsSleeping())
			{
				continue;
			}

			for (auto& other : m_pColliders)
			{
				if (collider == other)
				{
					continue;
				}

				if (other->IsSleeping())
				{
					continue; 
				}

				if (collider->IsStatic() && other->IsStatic())
				{
					continue;
				}

				if (collider->IsOverlapping(other->GetRect()))
				{
					other->SetOverlapping(true);
				}
			}
		}
	}
}

void dae::CollisionManager::SetDirty()
{
	m_IsDirty = true;
}