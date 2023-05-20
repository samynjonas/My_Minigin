#include "RenderComponent.h"
#include "ResourceManager.h"

#include "Renderer.h"
#include "GameObject.h"
#include "TransformComponent.h"

dae::RenderComponent::RenderComponent()
	: Component()
{

}


dae::RenderComponent::~RenderComponent()
{

}

void dae::RenderComponent::Update(float)
{

}

void dae::RenderComponent::Render()
{
	if (!m_pTexture)
	{
		return;
	}

	const auto& pos = GetOwner()->transform()->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}