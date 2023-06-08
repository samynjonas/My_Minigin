#include "CollisionManager.h"
#include <algorithm>

#include "BoxColliderComponent.h"

#include "servicelocator.h"
#include "Logger.h"

void dae::CollisionManager::RegisterCollider(BoxColliderComponent* collider, std::string layer)
{
	//Checking if the layer is valid
	int layerID{ LayerToID(layer, false) };
	if (layerID == -1)
	{
		layerID = AddLayer(layer);
	}

	m_ColliderLinkedLayer.push_back({ layerID });
	m_pColliders.push_back(collider);
	m_IsDirty = true;
}

void dae::CollisionManager::RegisterCollider(BoxColliderComponent* collider, std::vector<std::string> layers)
{
	//Checking if the layer is valid
	std::vector<int> vexLayerIDs;
	for (auto& layer : layers)
	{
		int layerID{ LayerToID(layer, false) };
		if (layerID == -1)
		{
			layerID = AddLayer(layer);
		}
		vexLayerIDs.push_back(layerID);
	}

	m_ColliderLinkedLayer.push_back(vexLayerIDs);
	m_pColliders.push_back(collider);
	m_IsDirty = true;
}

void dae::CollisionManager::UnregisterCollider(BoxColliderComponent* collider)
{
	for (size_t index = 0; index < m_pColliders.size(); index++)
	{
		if (m_pColliders[index] == collider)
		{
			m_pColliders.erase(m_pColliders.begin() + index);
			m_ColliderLinkedLayer.erase(m_ColliderLinkedLayer.begin() + index);
		}
	}
	m_IsDirty = true;
}

int dae::CollisionManager::AddLayer(const std::string& newLayer)
{
	m_Layers.push_back(newLayer);
	return static_cast<int>(m_Layers.size()) - 1;
}
std::string dae::CollisionManager::IntToLayer(int layerID, bool returnValid) const
{
	if (layerID >= static_cast<int>(m_Layers.size()) || layerID < 0)
	{		
		if (returnValid)
		{
			servicelocator<Logger>::get_serviceLocator().LogError({ std::to_string(layerID) + " has not been found in layers! Default layer has been returned!" });
			return m_Layers[0];
		}
		return "";
	}
	return m_Layers[layerID];
}
int dae::CollisionManager::LayerToID(std::string& layerName, bool returnValid) const
{
	for (int index = 0; index < m_Layers.size(); index++)
	{
		if (m_Layers[index] == layerName)
		{
			return index;
		}
	}

	if (returnValid)
	{
		servicelocator<Logger>::get_serviceLocator().LogError({ layerName + " has not been found in layers! Default layer has been returned!" });
		return 0;
	}
	return -1;
}

void dae::CollisionManager::Update()
{
	if (m_IsDirty)
	{
		for (auto& collider : m_pColliders) //Running through twice - should be more effecient
		{
			collider->SetOverlapping(false);
		}

		for (size_t index = 0; index < m_pColliders.size(); index++)
		{
			if (m_pColliders[index]->IsSleeping())
			{
				continue;
			}

			for (size_t otherIndex = 0; otherIndex < m_pColliders.size(); otherIndex++)
			{
				if (index == otherIndex) //Is the same collider
				{
					continue;
				}

				if (m_pColliders[otherIndex]->IsSleeping()) // No need to check a sleeping collider
				{
					continue;
				}

				if (m_pColliders[index]->IsStatic() && m_pColliders[otherIndex]->IsStatic()) //No need to check 2 static colliders
				{
					continue;
				}

				if (HasSharedLayer(index, otherIndex) == false)
				{
					continue;
				}

				if (m_pColliders[index]->IsOverlapping(m_pColliders[otherIndex]->GetRect()))
				{
					m_pColliders[otherIndex]->SetOverlapping(true);
				}
			}
		}


		//for (auto& collider : m_pColliders)
		//{
		//	if (collider->IsSleeping())
		//	{
		//		continue;
		//	}
		//
		//	for (auto& other : m_pColliders)
		//	{
		//		if (collider == other)
		//		{
		//			continue;
		//		}
		//
		//		if (other->IsSleeping())
		//		{
		//			continue; 
		//		}
		//
		//		if (collider->IsStatic() && other->IsStatic())
		//		{
		//			continue;
		//		}
		//
		//		if (collider->IsOverlapping(other->GetRect()))
		//		{
		//			other->SetOverlapping(true);
		//		}
		//	}
		//}
	}
}

bool dae::CollisionManager::HasSharedLayer(size_t collIndex, size_t otherCollIndex) const
{
	if (collIndex < 0 || otherCollIndex < 0)
	{
		return false;
	}
	if (collIndex >= m_ColliderLinkedLayer.size() || otherCollIndex >= m_ColliderLinkedLayer.size())
	{
		return false;
	}

	for (size_t i = 0; i < m_ColliderLinkedLayer[collIndex].size(); i++)
	{
		auto result = std::find(m_ColliderLinkedLayer[otherCollIndex].begin(), m_ColliderLinkedLayer[otherCollIndex].end(), m_ColliderLinkedLayer[collIndex][i]);
		if (result != m_ColliderLinkedLayer[otherCollIndex].end())
		{
			return true;
		}
	}
	return false;
}

void dae::CollisionManager::SetDirty()
{
	m_IsDirty = true;
}