#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class RenderComponent;
	class CameraComponent;

	class RenderingManager final : public Singleton<RenderingManager>
	{
	public:
		void RegisterRenderer(RenderComponent* renderer, const short& depth);
		void UnregisterRenderer(RenderComponent* renderer);

		void Render();
		void CheckForDead();

		void SetActiveCamera(CameraComponent* pCamera);

		int GetRenderingMapSize() const
		{
			return static_cast<int>(m_MapRenderers.size());
		}

	private:
		friend class Singleton<RenderingManager>;
		RenderingManager() = default;

		std::unordered_map<short, std::vector<RenderComponent*>> m_MapRenderers;

		std::vector<RenderComponent*> m_VecRenderings;

		CameraComponent* m_pActiveCamera{ nullptr };
	};
}



