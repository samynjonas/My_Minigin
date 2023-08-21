#pragma once
#include "SceneManager.h"
#include "subject.h"

#include "GameObject.h"

namespace dae
{
	class RenderingManager;
	class GameObject;

	class Scene final : public Subject
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::unique_ptr<GameObject> object);
		void Remove(std::unique_ptr<GameObject> object);
		void RemoveAll();

		void Update();
		void Render() const;

		void LoadScene();
		void UnloadScene();

		void ActivateAll();
		void DeActivateAll();

		void Cleanup();

		std::string GetSceneName() const
		{
			return m_name;
		}
		const std::vector<std::unique_ptr<GameObject>>& GetGameObject() const
		{
			return m_objects;
		}

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector< std::unique_ptr<GameObject> > m_objects{};
		std::vector<RenderComponent*> m_vecRender;

		static unsigned int m_idCounter;
	};
}
