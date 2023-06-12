#include "SceneManager.h"
#include "Scene.h"
#include "servicelocator.h"
#include "Logger.h"

void dae::SceneManager::Update()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Update();
	}
	else
	{
		if (m_SceneIDs.empty())
		{
			return;
		}
		LoadScene(m_SceneIDs[0]);
	}
}

void dae::SceneManager::Render()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	
	m_scenes.push_back(scene);
	m_SceneIDs.push_back(name);

	return *scene;
}

dae::Scene* dae::SceneManager::LoadScene(const std::string& name)
{
	int sceneID = StringToID(name);

	if (sceneID < 0 || sceneID >= m_SceneIDs.size())
	{
		return nullptr;
	}

	servicelocator<Logger>::get_serviceLocator().LogWarning({ "Loading scene: " + std::to_string(sceneID) });
	if (m_pActiveScene)
	{
		m_pActiveScene->UnloadScene();
	}
	
	m_pActiveScene = m_scenes[sceneID].get();
	
	m_pActiveScene->LoadScene();
	return m_pActiveScene;
}

int dae::SceneManager::StringToID(const std::string& name, bool returnValid) const
{
	for (int index = 0; index < m_SceneIDs.size(); index++)
	{
		if (m_SceneIDs[index] == name)
		{
			return index;
		}
	}

	servicelocator<Logger>::get_serviceLocator().LogError({ name + " is not a valid scene" });
	if (returnValid)
	{
		return 0;
	}
	return -1;
}

std::string dae::SceneManager::GetActiveSceneName() const
{
	if (m_pActiveScene == nullptr)
	{
		return "";
	}
	return m_pActiveScene->GetSceneName();
}

void dae::SceneManager::RemoveScene(const std::string& name)
{
	int sceneID = StringToID(name, false);
	if (sceneID == -1)
	{
		return;
	}

	m_scenes[sceneID]->RemoveAll();
	m_scenes.erase(m_scenes.begin() + sceneID);
	m_SceneIDs.erase(m_SceneIDs.begin() + sceneID);
}
