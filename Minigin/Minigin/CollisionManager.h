#pragma once
#include "Singleton.h"
#include <vector>

namespace dae
{
	class BoxColliderComponent;
	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void RegisterCollider(BoxColliderComponent* collider);
		void UnregisterCollider(BoxColliderComponent* collider);

		void Update();
		void SetDirty();

	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;

		bool m_IsDirty{ false }; //Only check for collision of dirty, this will only be done when a new collider is added or movement has happend on at least one collider
		std::vector<BoxColliderComponent*> m_pColliders;
	};
}



