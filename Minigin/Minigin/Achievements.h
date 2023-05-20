#pragma once
#include "Observer.h"

namespace dae
{
	class Achievements final : public Observer
	{
	public:
		virtual void Notify(Event event, subject* actor) override;


	private:

	};
}


