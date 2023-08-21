#pragma once
#include "Component.h"
#include <string>
#include <memory>

#include <glm/glm.hpp>

namespace dae
{
	class Texture2D;
	class RenderComponent final : public Component
	{
	public:		
		RenderComponent();
		~RenderComponent();
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		void Update() override;	

		void Render(glm::vec3 transformPosition);

		void Initialize(short depth = 1, bool isUI = false);

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> pTexture);

		glm::vec2 GetTextureDimensions() const;

	private:
		std::shared_ptr<Texture2D> m_pTexture{ nullptr };
		bool m_IsUI{ false };

	};
}


