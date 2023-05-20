#pragma once
#include <memory>
#include <vector>

namespace dae
{
	class Texture2D;
	class Component;
	class RenderComponent;
	class TransformComponent;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void Initialize(const std::string& name);

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

		RenderComponent* renderer() const
		{
			return m_pRenderComponent;
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
			const T* pComponent{ GetComponent<T>() };

			// If this component exists, destroy it
			if (pComponent)
			{
				// Mark the component as dead
			}
		}

	private:
		std::string m_Name{};

		//Components
		std::vector<std::unique_ptr<Component>> m_vecComponents;
		TransformComponent* m_pTransformComponent{ nullptr };
		RenderComponent* m_pRenderComponent{ nullptr };




	};
}
