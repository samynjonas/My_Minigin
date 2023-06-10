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
		enum class RenderOrder
		{
			normal,
			gui
		};
		
		RenderComponent();
		~RenderComponent();
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		void Update() override;
		
		void Render();
		void GUIRender();

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> pTexture);


		void SetRenderOrder(RenderOrder newRenderOrder)
		{
			m_RenderOrder = newRenderOrder;
		}

		glm::vec2 GetTextureDimensions() const;

	private:
		RenderOrder m_RenderOrder{ RenderOrder::normal };

		std::shared_ptr<Texture2D> m_pTexture{};

	};
}


