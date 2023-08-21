#pragma once
#include "Component.h"
#include <string>
#include <memory>

#include "glm/glm.hpp"

#include "ResourceManager.h"

namespace dae
{
	class LeaderbordComponent final : public Component
	{
	public:	
		LeaderbordComponent() = default;
		~LeaderbordComponent() = default;

		LeaderbordComponent(const LeaderbordComponent& other) = delete;
		LeaderbordComponent(LeaderbordComponent&& other) = delete;
		LeaderbordComponent& operator=(const LeaderbordComponent& other) = delete;
		LeaderbordComponent& operator=(LeaderbordComponent&& other) = delete;

		void DisplayLeaderbord();

		void Update() override {};

	private:
		const int MAX_SCORES_DISPLAYED{ 10 };
		void AddText(glm::vec2 position, const std::string& text, std::shared_ptr<dae::Font> font);
	
	};
}

