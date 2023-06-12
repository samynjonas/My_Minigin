#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void RemoveScene(const std::string& name);

		Scene* LoadScene(const std::string& name);
		int StringToID(const std::string& name, bool returnValid = true) const;

		void Update();
		void Render();

		const std::vector<std::string>& GetSceneNames() const
		{
			return m_SceneIDs;
		}
		std::string GetActiveSceneName() const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::vector<std::string> m_SceneIDs;

		Scene* m_pActiveScene{ nullptr };
	};
}
