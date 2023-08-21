#include "RenderComponent.h"
#include "ResourceManager.h"

#include "Renderer.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Texture2D.h"

#include "Scene.h"
#include "RenderingManager.h"

dae::RenderComponent::RenderComponent()
	: Component()
{

}


dae::RenderComponent::~RenderComponent()
{
	RenderingManager::GetInstance().UnregisterRenderer(this);
}

void dae::RenderComponent::Update()
{

}

void dae::RenderComponent::Render(glm::vec3 transformPosition)
{
	if (!m_pTexture)
	{
		return;
	}

	auto pos = GetOwner()->transform()->GetWorldPosition();

	//Applying transformation
	if (m_IsUI == false)
	{
		pos.x += transformPosition.x;
		pos.y += transformPosition.y;
	}

	const auto& rot = GetOwner()->transform()->GetWorldRotation();

	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, rot);
}

void dae::RenderComponent::Initialize(short depth, bool isUI)
{
	m_IsUI = isUI;
	RenderingManager::GetInstance().RegisterRenderer(this, depth);
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);		
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D> pTexture)
{
	m_pTexture = pTexture;
}

glm::vec2 dae::RenderComponent::GetTextureDimensions() const
{
	glm::vec2 dim{};
	if (m_pTexture == nullptr)
	{
		return dim;
	}

	auto textureSize = m_pTexture.get()->GetSize();
	dim.x = static_cast<float>(textureSize.x);
	dim.y = static_cast<float>(textureSize.y);

	return dim;
}