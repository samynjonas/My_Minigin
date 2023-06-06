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

dae::GameObject::~GameObject() = default;

void GameObject::Initialize(const std::string& name)
{
	m_Name = name;

	//These need to be a part of every gameobject
	m_pTransformComponent	= AddComponent<TransformComponent>();
	m_pRenderComponent		= AddComponent<RenderComponent>();
}

void GameObject::Update(float deltaTime)
{
	for (const auto& pComponent : m_vecComponents)
	{
		pComponent->Update(deltaTime);		
	}
}

void GameObject::Render() const
{
	if (m_pRenderComponent)
	{
		m_pRenderComponent->Render();
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
