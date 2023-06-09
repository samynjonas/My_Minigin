#pragma once
#include "Component.h"
#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "subject.h"

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

		int minX() const
		{
			return _x;
		}
		int minY() const
		{
			return _y;
		}
		int maxX() const
		{
			return _x + _width;
		}
		int maxY() const
		{
			return _y + _height;
		}

		int GetPosHalfWidth() const
		{
			return _x + _width / 2;
		}
		int GetPosHalfHeight() const
		{
			return _y + _height / 2;
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

		void Initialize(int x, int y, int width, int height, bool isStatic = false, std::string Layer = "Default", std::vector<std::string> collideLayers = {"Default"}, std::vector<std::string> skipLayers = {"None"});
		void Initialize(int width, int height, bool isStatic = false, std::string Layer = "Default", std::vector<std::string> collideLayers = { "Default" }, std::vector<std::string> skipLayers = { "None" });
		void Initialize(bool isStatic = false, std::string Layer = "Default", std::vector<std::string> collideLayers = { "Default" }, std::vector<std::string> skipLayers = { "None" });

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

		void SetOverlapping(bool isOverlapping, glm::vec2 collisionPoint = {});
		
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

		glm::vec2 GetCollisionPoint() const
		{
			return m_CollisionPoint;
		}

	private:
		std::unique_ptr<Rect> m_pColliderRect{ nullptr };
		glm::vec2 m_PositionOffset{};

		glm::vec2 m_CollisionPoint{};

		bool m_IsSleeping{ false };		
		bool m_IsTrigger{ false };	//Trigger just give a signal - Colliders bounce back	
		bool m_IsStatic{ false };	//STATIC COLLIDERS DONT CHECK ON OTHER STATIC COLLDIDERS - THEY CANT MOVE
		
		bool m_IsOverlapping{ false };	//This frame
		bool m_WasOverlapping{ false }; //Last frame

		bool m_IsDirty{ false };

		std::string m_Layer{};

		//TODO give a collider a single layer, you can specify against which layers you want it to collide

		void UpdateTransform();
		void estimateCollisionPoint(const Rect* other);

	};
}


