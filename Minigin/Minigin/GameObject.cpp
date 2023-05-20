#include <string>
#include "GameObject.h"

#include "ResourceManager.h"
#include "Renderer.h"

#include "RenderComponent.h"
#include "TransformComponent.h"

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
