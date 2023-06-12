#include "CollisionManager.h"
#include <algorithm>

#include "BoxColliderComponent.h"
#include "GameObject.h"

#include "servicelocator.h"
#include "Logger.h"

void dae::CollisionManager::RegisterCollider(BoxColliderComponent* collider, std::string layer, std::vector<std::string> collideLayers, std::vector<std::string> skipLayers)
{
	//Colliders layer
	{
		int layerID{ LayerToID(layer, false) };
		if (layerID == -1)
		{
			layerID = AddLayer(layer);
		}

		m_ColliderLayer.push_back(layerID);
	}

	//Add layers that it collides with
	std::vector<int> vecLayerIDs;
	for (auto& _layer : collideLayers)
	{
		int layerID{ LayerToID(_layer, false) };
		if (layerID == -1)
		{
			layerID = AddLayer(_layer);
		}
		vecLayerIDs.push_back(layerID);
	}
	m_ColliderLinkedLayer.push_back(vecLayerIDs);

	//Add layers that it doesnt collide with
	std::vector<int> vecSkipLayerIDs;
	for (auto& _layer : skipLayers)
	{
		int layerID{ LayerToID(_layer, false) };
		if (layerID == -1)
		{
			layerID = AddLayer(_layer);
		}
		vecSkipLayerIDs.push_back(layerID);
	}
	m_ColliderSkipLayer.push_back(vecSkipLayerIDs);	

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
			m_ColliderLayer.erase(m_ColliderLayer.begin() + index);
			m_ColliderSkipLayer.erase(m_ColliderSkipLayer.begin() + index);
			m_ColliderLinkedLayer.erase(m_ColliderLinkedLayer.begin() + index);
		}
	}
	m_IsDirty = true;
}
void dae::CollisionManager::CheckForDeadColliders()
{
	for (size_t index = 0; index < m_pColliders.size(); index++)
	{
		if (m_pColliders[index]->GetOwner()->IsMarkedForDead())
		{
			m_pColliders.erase(m_pColliders.begin() + index);
			m_ColliderLayer.erase(m_ColliderLayer.begin() + index);
			m_ColliderSkipLayer.erase(m_ColliderSkipLayer.begin() + index);
			m_ColliderLinkedLayer.erase(m_ColliderLinkedLayer.begin() + index);
			
			m_IsDirty = true;
		}
	}
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
int dae::CollisionManager::LayerToID(const std::string& layerName, bool returnValid) const
{
	for (int index = 0; index < static_cast<int>(m_Layers.size()); index++)
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

				if (ContainsLayer(m_ColliderLayer[otherIndex], index, m_ColliderLinkedLayer) == false)
				{
					continue;
				}

				if (m_pColliders[index]->IsOverlapping(m_pColliders[otherIndex]->GetRect()))
				{
					m_pColliders[otherIndex]->SetOverlapping(true, m_pColliders[index]->GetCollisionPoint());
				}
			}
		}

		m_IsDirty = false;
	}
}

bool dae::CollisionManager::HasSharedLayer(size_t collIndex, size_t otherCollIndex, const std::vector<std::vector<int>>& vecLayers) const
{
	if (collIndex < 0 || otherCollIndex < 0)
	{
		return false;
	}
	if (collIndex >= vecLayers.size() || otherCollIndex >= vecLayers.size())
	{
		return false;
	}

	for (size_t i = 0; i < vecLayers[collIndex].size(); i++)
	{
		auto result = std::find(vecLayers[otherCollIndex].begin(), vecLayers[otherCollIndex].end(), vecLayers[collIndex][i]);
		if (result != vecLayers[otherCollIndex].end())
		{
			return true;
		}
	}
	return false;
}

bool dae::CollisionManager::ContainsLayer(size_t collIndex, std::vector<std::string> layers, const std::vector<std::vector<int>>& vecLayers) const
{
	if (collIndex < 0)
	{
		return false;
	}
	if (collIndex >= vecLayers.size())
	{
		return false;
	}

	for (size_t i = 0; i < layers.size(); i++)
	{
		auto result = std::find(vecLayers[collIndex].begin(), vecLayers[collIndex].end(), LayerToID(layers[i], false));
		if (result != vecLayers[collIndex].end())
		{
			return true;
		}
	}
	return false;
}
bool dae::CollisionManager::ContainsLayer(size_t collIndex, std::vector<int> layers, const std::vector<std::vector<int>>& vecLayers) const
{
	if (collIndex < 0)
	{
		return false;
	}
	if (collIndex >= vecLayers.size())
	{
		return false;
	}

	for (size_t i = 0; i < layers.size(); i++)
	{
		auto result = std::find(vecLayers[collIndex].begin(), vecLayers[collIndex].end(), layers[i]);
		if (result != vecLayers[collIndex].end())
		{
			return true;
		}
	}
	return false;
}
bool dae::CollisionManager::ContainsLayer(int colliderLayer, size_t otherIndex, const std::vector<std::vector<int>>& vecLayers) const
{
	if (vecLayers.empty())
	{
		return false;
	}

	if (otherIndex >= vecLayers.size() || otherIndex < 0)
	{
		return false;
	}

	for (const auto& otherlayer : vecLayers[otherIndex])
	{
		if (otherlayer == colliderLayer)
		{
			return true;
		}
	}

	return false;
}
bool dae::CollisionManager::ContainsLayer(int colliderLayer, const std::vector<std::string> layers) const
{
	for (const auto& layer : layers)
	{
		if (colliderLayer == LayerToID(layer))
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

bool dae::CollisionManager::Raycast(glm::vec2 origin, dae::Directions direction, dae::RaycastInfo& hitinfo, int maxDistance, int minDistance, std::vector<std::string> layers, std::vector<std::string>) const
{
	//Locked the raycast to a single dimensions - no diagonal raycasters, sorry
	if (maxDistance == -1)
	{
		maxDistance = MAX_RAYCAST_DISTANCE;
	}
	
	int collisionDistance{ maxDistance };
	int closestCollider{ -1 };

	for (size_t index = 0; index < m_pColliders.size(); index++)
	{
		if (m_pColliders[index]->IsSleeping())
		{
			continue;
		}

		bool isLeftRight{ false };
		switch (direction)
		{
		case dae::Directions::Left:
		default:
		{
			if (m_pColliders[index]->GetRect()->_x > origin.x)
			{
				continue;
			}
			isLeftRight = true;
		}
		break;
		case dae::Directions::Right:
		{
			if (m_pColliders[index]->GetRect()->maxX() < origin.x)
			{
				continue;
			}
			isLeftRight = true;
		}
		break;
		case dae::Directions::Above:
		{
			if (m_pColliders[index]->GetRect()->_y > origin.y)
			{
				continue;
			}
		}
		break;
		case dae::Directions::Below:
		{
			if (m_pColliders[index]->GetRect()->maxY() < origin.y)
			{
				continue;
			}
		}
		break;
		}

		if (isLeftRight)
		{
			//std::cout << "raycastorigin: " << origin.y << std::endl;
			//std::cout << "Tank: " << m_pColliders[index]->GetRect()->maxY() << " : " << m_pColliders[index]->GetRect()->_y << std::endl;

			if (m_pColliders[index]->GetRect()->maxY() < origin.y)
			{
				continue;
			}

			if (m_pColliders[index]->GetRect()->_y > origin.y)
			{
				continue;
			}

			int _distance = abs(m_pColliders[index]->GetRect()->GetPosHalfWidth() - static_cast<int>(origin.x));
			if (_distance < collisionDistance && _distance > minDistance)
			{
				collisionDistance = _distance;
				closestCollider = static_cast<int>(index);
			}
		}
		else
		{
			if (m_pColliders[index]->GetRect()->maxX() < origin.x)
			{
				continue;
			}

			if (m_pColliders[index]->GetRect()->_x > origin.x)
			{
				continue;
			}

			int _distance = abs(m_pColliders[index]->GetRect()->GetPosHalfHeight() - static_cast<int>(origin.y));
			if (_distance < collisionDistance && _distance > minDistance)
			{
				collisionDistance = _distance;
				closestCollider = static_cast<int>(index);
			}
		}
	}


	if (closestCollider == -1)
	{
		return false;
	}

	hitinfo.distance = collisionDistance;
	hitinfo.hitCollider = m_pColliders[closestCollider];

	if (collisionDistance < minDistance)
	{
		return false;
	}

	if (ContainsLayer(m_ColliderLayer[closestCollider], layers) == false)
	{
		return false;
	}
	
	return true;
}