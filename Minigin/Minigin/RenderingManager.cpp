#include "RenderingManager.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "GameObject.h"

namespace dae
{
	void RenderingManager::RegisterRenderer(RenderComponent* renderer, const short& depth)
	{
		auto foundIterator = m_MapRenderers.find(depth);
		if (foundIterator != m_MapRenderers.end())
		{
			foundIterator->second.push_back(renderer);
		}
		else
		{
			m_MapRenderers[depth].push_back(renderer);
		}

		m_VecRenderings.push_back(renderer);
	}

	void RenderingManager::SetActiveCamera(CameraComponent* pCamera)
	{
		m_pActiveCamera = pCamera;
	}

	void RenderingManager::UnregisterRenderer(RenderComponent* renderer)
	{
		for (auto& vecRendererPair : m_MapRenderers)
		{
			auto& rendererVector = vecRendererPair.second;

			auto it = std::find(rendererVector.begin(), rendererVector.end(), renderer);
			if (it != rendererVector.end())
			{
				rendererVector.erase(it);
				return;
			}
		}
	}

	void RenderingManager::CheckForDead()
	{
		// Perform any necessary cleanup or checks for "dead" renderers here.
	}

	void RenderingManager::Render()
	{
		glm::vec3 transformVector{};
		if (m_pActiveCamera)
		{
			transformVector = glm::vec3(m_pActiveCamera->GetViewMatrix()[3]);
		}

		for (auto& vecRendererPair : m_MapRenderers)
		{
			auto& rendererVector = vecRendererPair.second;

			for (auto& renderer : rendererVector)
			{
				if (renderer->GetOwner() && renderer->GetOwner()->IsActive() && !renderer->GetOwner()->IsMarkedForDead())
				{
					renderer->Render(transformVector);
				}
			}
		}
	}
}
