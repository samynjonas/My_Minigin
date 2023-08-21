#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	struct rect 
	{
		int x{};
		int y{};
		int width{};
		int height{};
	};

	class TeleportComponent final : public Component, public Observer
	{
	public:
		TeleportComponent();
		~TeleportComponent();

		TeleportComponent(const TeleportComponent& other) = delete;
		TeleportComponent(TeleportComponent&& other) = delete;
		TeleportComponent& operator=(const TeleportComponent& other) = delete;
		TeleportComponent& operator=(TeleportComponent&& other) = delete;

		void Initialize(rect mapRect);
		void Update() override;

		void Notify(Event currEvent, Subject* actor);

	private:
		void TeleportToRandomPosition();

		rect m_MapRect{};
	};
}

