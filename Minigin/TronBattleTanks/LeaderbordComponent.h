#pragma once
#include "Component.h"

namespace dae
{
	class LeaderbordComponent
	{
	public:
		LeaderbordComponent();
		~LeaderbordComponent();

		LeaderbordComponent(const LeaderbordComponent& other) = delete;
		LeaderbordComponent(LeaderbordComponent&& other) = delete;
		LeaderbordComponent& operator=(const LeaderbordComponent& other) = delete;
		LeaderbordComponent& operator=(LeaderbordComponent&& other) = delete;

	private:

	};
}



