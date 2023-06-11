#pragma once
#include "Singleton.h"
#include <vector>
#include <string>

#include "glm/glm.hpp"

namespace dae
{
	class BoxColliderComponent;

	struct RaycastInfo
	{
		int distance{ 0 };
		BoxColliderComponent* hitCollider{ nullptr };
	};
	enum class Directions
	{
		Left,
		Right,
		Above,
		Below
	};

	//TODO Check if it is a scene
	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void RegisterCollider(BoxColliderComponent* collider, std::string layer, std::vector<std::string> collideLayers, std::vector<std::string> skipLayers);

		void UnregisterCollider(BoxColliderComponent* collider);
		void CheckForDeadColliders();

		void Update();
		void SetDirty();

		int AddLayer(const std::string& newLayer);

		std::string IntToLayer(int layerID, bool returnValid = true) const;
		int LayerToID(const std::string& layerName, bool returnValid = true) const;

		bool Raycast(glm::vec2 origin, Directions direction, RaycastInfo& hitinfo, int maxDistance = -1, int minDistance = -1, std::vector<std::string> layers = { "Default" }, std::vector<std::string> skipLayers = {"None"}) const;

	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;

		const int MAX_RAYCAST_DISTANCE{ 750 };
		const int MIN_RAYCAST_DISTANCE{ 10 };
		
		bool m_IsDirty{ false }; //Only check for collision of dirty, this will only be done when a new collider is added or movement has happend on at least one collider
		std::vector<BoxColliderComponent*> m_pColliders;


		bool HasSharedLayer(size_t collIndex, size_t otherCollIndex, const std::vector<std::vector<int>>& layers) const;
		bool ContainsLayer(size_t collIndex, std::vector<std::string> layers, const std::vector<std::vector<int>>& vecLayers) const;
		bool ContainsLayer(size_t collIndex, std::vector<int> layers, const std::vector<std::vector<int>>& vecLayers) const;
		bool ContainsLayer(int colliderLayer, size_t otherIndex, const std::vector<std::vector<int>>& vecLayers) const;
		bool ContainsLayer(int colliderLayer, const std::vector<std::string> layers) const;

		//For user interfacing strings are easier - for logic int are less expensive
		std::vector<std::string> m_Layers{ "None", "Default" };
		
		std::vector<int> m_ColliderLayer;
		std::vector<std::vector<int>> m_ColliderLinkedLayer;
		std::vector<std::vector<int>> m_ColliderSkipLayer;
	
	};
}



