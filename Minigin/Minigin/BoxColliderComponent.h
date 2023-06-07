#pragma once
#include "Component.h"
#include <memory>
#include "subject.h"
#include "glm/glm.hpp"

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

		void Initialize(int x, int y, int width, int height, bool isStatic = false);
		void Initialize(int width, int height, bool isStatic = false);
		void Initialize(bool isStatic = false);

		void Update() override;

		Rect* GetRect() const
		{
			return m_pColliderRect.get();
		}

		bool IsOverlapping(const Rect* other);

		void OnCollisionEnter()
		{
			if (m_IsTrigger)
			{
				NotifyObservers(TriggerEnter, this);
			}
			else
			{
				NotifyObservers(CollisionEnter, this);
			}
		}
		void OnCollisionExit()
		{
			if (m_IsTrigger)
			{
				NotifyObservers(TriggerExit, this);
			}
			else
			{
				NotifyObservers(CollisionExit, this);
			}
		}

		void SetOverlapping(bool isOverlapping);
		
		bool IsSleeping() const
		{
			return m_IsSleeping;
		}
		bool IsStatic() const
		{
			return m_IsStatic;
		}

		void PutToSleep();
		void WakeUp();

	private:
		std::unique_ptr<Rect> m_pColliderRect{ nullptr };
		glm::vec2 m_PositionOffset{};

		bool m_IsSleeping{ false };		
		bool m_IsTrigger{ false };	//Trigger just give a signal - Colliders bounce back	
		bool m_IsStatic{ false };	//STATIC COLLIDERS DONT CHECK ON OTHER STATIC COLLDIDERS - THEY CANT MOVE
		
		bool m_IsOverlapping{ false };	//This frame
		bool m_WasOverlapping{ false }; //Last frame

		bool m_IsDirty{ false };


		void UpdateTransform();
	};
}


