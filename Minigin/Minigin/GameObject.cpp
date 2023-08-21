#include <string>
#include "GameObject.h"

#include "ResourceManager.h"
#include "Renderer.h"

#include "RenderComponent.h"
#include "TransformComponent.h"

#include "servicelocator.h"
#include "Logger.h"

#include <iostream>

using namespace dae;

dae::GameObject::~GameObject()
{
	for (auto& child : m_pChildren)
	{
		child->MarkForDead();
	}
}

void GameObject::Initialize(const std::string& name, Scene* pScene)
{
	m_Name = name;

	m_pScene = pScene;

	//These need to be a part of every gameobject
	m_pTransformComponent	= AddComponent<TransformComponent>();
}

void GameObject::Update()
{
	if (m_IsMarkedForDead)
	{
		return;
	}


	for (const auto& pComponent : m_vecComponents)
	{
		if (m_IsMarkedForDead)
		{
			return;
		}

		if (pComponent)
		{
			pComponent->Update();
		}
	}
}

void GameObject::LateUpdate()
{
	for (const auto& pComponent : m_vecComponents)
	{
		if (m_IsMarkedForDead)
		{
			return;
		}

		if (pComponent)
		{
			pComponent->LateUpdate();
		}
	}
}

void GameObject::SetParent(GameObject* pParent)
{	
	if (pParent == m_pParent)
	{
		//Parent was already set
		servicelocator<dae::Logger>::get_serviceLocator().LogWarning("Trying to set parent, but parent was already set with same GameObject");
		return;
	}
	
	
	GameObject* pPrevParent{ m_pParent };
	if (pPrevParent)
	{
		//Remove itself from the children list of the previous parent
		for (size_t i = 0; i < pPrevParent->m_pChildren.size(); i++)
		{
			auto& pChild = pPrevParent->m_pChildren[i];

			if (pChild == this)
			{
				//Setting new parent
				if (pParent)
				{
					pParent->m_pChildren.push_back(pChild);
				}

				//Removing from old
				pPrevParent->m_pChildren[i] = pPrevParent->m_pChildren.back();
				pPrevParent->m_pChildren.pop_back();
				break;
			}
		}
	}
	else
	{
		//If there was no parent previously - delete child from scene and add to parent
		pParent->m_pChildren.push_back(this);
	}
	
	//Set new parent
	m_pParent = pParent;
	
	
	if (m_pParent)
	{
		//Set local position relative to the new parent position
		auto parentTransfrom = m_pParent->transform();
		if (parentTransfrom)
		{
			transform()->SetLocalPosition(transform()->GetWorldPosition() - parentTransfrom->GetWorldPosition());
		}
	}
	else
	{
		//Local position becomes world position
		transform()->SetLocalPosition( transform()->GetWorldPosition() );
	}
	
}
