#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace dae
{
	class Texture2D;
	class Component;
	class RenderComponent;
	class TransformComponent;
	class Scene;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		virtual void Update();
		virtual void LateUpdate();

		//virtual void Render() const;
		//virtual void LateRender() const;

		void Initialize(const std::string& name, Scene* pScene);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		TransformComponent* transform() const
		{
			return m_pTransformComponent;
		}

		template <class T>
		T* GetComponent() const
		{
			static_assert(std::is_base_of<Component, T>(), "Needs to be component");

			for (const auto& pComponent : m_vecComponents)
			{
				T* derivedComponent{ dynamic_cast<T*>(pComponent.get()) };

				if (derivedComponent) return derivedComponent;
			}

			return nullptr;
		}

		template <class T>
		T* GetComponentInChildren() const
		{
			static_assert(std::is_base_of<Component, T>(), "Needs to be component");

			for (const auto& child : m_pChildren)
			{
				for (const auto& pComponent : child->m_vecComponents)
				{
					T* derivedComponent{ dynamic_cast<T*>(pComponent.get()) };

					if (derivedComponent) return derivedComponent;
				}
			}			

			return nullptr;
		}

		template <class T>
		T* AddComponent()
		{
			static_assert(std::is_base_of<Component, T>(), "Needs to be component");

			auto pComponent = std::make_unique<T>();

			pComponent->SetOwner(this);

			T* pComponentPtr{ pComponent.get() };

			m_vecComponents.push_back(std::move(pComponent));

			return pComponentPtr;
		}

		template <class T>
		void RemoveComponent()
		{
			static_assert(std::is_base_of<Component, T>(), "Needs to be component");

			for (auto itr{m_vecComponents.begin()}; itr < m_vecComponents.end(); ++itr)
			{
				T* component{ dynamic_cast<T*>((*itr).get()) };
				if (component != nullptr)
				{
					m_vecComponents.erase(itr);
					return;
				}
			}
		}

		Scene* GetScene() const
		{
			return m_pScene;
		}
		
		void SetParent(GameObject* pParent);
		GameObject* GetParent()
		{
			return m_pParent;
		}
		std::vector<GameObject*> GetChildren() const
		{
			return m_pChildren;
		}

		void MarkForDead()
		{
			//std::cout << "MarkForDead: " << GetName() << std::endl; //TODO Remove

			m_IsMarkedForDead = true;
		}
		bool IsMarkedForDead() const
		{
			return m_IsMarkedForDead;
		}

		std::string GetName() const
		{
			return m_Name;
		}

		void SetActiveState(bool value)
		{
			m_IsActive = value;
			for (auto& child : m_pChildren)
			{
				child->SetActiveState(value);
			}
		}

		bool IsActive() const
		{
			return m_IsActive;
		}

	private:
		std::string m_Name{};

		Scene* m_pScene{};

		bool m_IsMarkedForDead{ false };
		bool m_IsActive{ false };

		//Components
		std::vector<std::unique_ptr<Component>> m_vecComponents;
		TransformComponent* m_pTransformComponent{ nullptr };

		GameObject* m_pParent{ nullptr };
		std::vector<GameObject*> m_pChildren;
	};
}
