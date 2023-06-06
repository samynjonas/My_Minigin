#pragma once
#include "Component.h"
#include <memory>
#include "subject.h"

#include <iostream>

namespace dae
{
	struct Rect
	{
		int _x;
		int _y;
		int _width;
		int _height;

		Rect(int x, int y, int width, int height)
			: _x{ x }
			, _y{ y }
			, _width{ width }
			, _height{ height }
		{
		}
	};


	class BoxColliderComponent final : public Component, public subject
	{
	public:
		BoxColliderComponent();
		~BoxColliderComponent();

		BoxColliderComponent(const BoxColliderComponent& other) = delete;
		BoxColliderComponent(BoxColliderComponent&& other) = delete;
		BoxColliderComponent& operator=(const BoxColliderComponent& other) = delete;
		BoxColliderComponent& operator=(BoxColliderComponent&& other) = delete;

		void Initialize(int x, int y, int width, int height);
		void Update() override;

		Rect* GetRect() const
		{
			return m_pColliderRect.get();
		}

		bool IsOverlapping(const Rect* other);

		void OnCollisionEnter()
		{
			std::cout << "Collision enter" << std::endl;

			if (m_IsTrigger)
			{
				NotifyObservers(TriggerEnter, this);
			}
			else
			{
				NotifyObservers(CollisionEnter, this);
			}
		}

		void SetOverlapping(bool isOverlapping);

	private:
		std::unique_ptr<Rect> m_pColliderRect{ nullptr };

		bool m_IsTrigger{ false };
		bool m_IsOverlapping{ false };
	
		void UpdateTransform();
	};
}


