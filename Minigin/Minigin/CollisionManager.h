#pragma once
#include "Singleton.h"
#include <vector>
#include <string>

namespace dae
{
	//TODO Check if it is a scene
	class BoxColliderComponent;
	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void RegisterCollider(BoxColliderComponent* collider, std::string layer = "Default");
		void RegisterCollider(BoxColliderComponent* collider, std::vector<std::string> layers);

		void UnregisterCollider(BoxColliderComponent* collider);

		void Update();
		void SetDirty();

		int AddLayer(const std::string& newLayer);

		std::string IntToLayer(int layerID, bool returnValid = true) const;
		int LayerToID(std::string& layerName, bool returnValid = true) const;

	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;

		bool m_IsDirty{ false }; //Only check for collision of dirty, this will only be done when a new collider is added or movement has happend on at least one collider
		std::vector<BoxColliderComponent*> m_pColliders;
		std::vector<std::vector<int>> m_ColliderLinkedLayer;

		bool HasSharedLayer(size_t collIndex, size_t otherCollIndex) const;

		//For user interfacing string are easier - for logic int are less expensive
		std::vector<std::string> m_Layers{ "Default" };
	};
}



