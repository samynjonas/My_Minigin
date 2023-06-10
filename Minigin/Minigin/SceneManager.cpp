#include "SceneManager.h"
#include "Scene.h"
#include "servicelocator.h"
#include "Logger.h"

void dae::SceneManager::Update()
{
	//for(auto& scene : m_scenes)
	//{
	//	scene->Update();
	//}

	if (m_pActiveScene)
	{
		m_pActiveScene->Update();
	}
}

void dae::SceneManager::Render()
{
	//for (const auto& scene : m_scenes)
	//{
	//	scene->Render();
	//}

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

	if (m_scenes.size() == 1)
	{
		//Is first scene
		m_pActiveScene = scene.get();
		m_pActiveScene->LoadScene();
	}

	return *scene;
}

void dae::SceneManager::LoadScene(const std::string& name)
{
	int sceneID = StringToID(name);

	if (sceneID < 0 || sceneID >= m_SceneIDs.size())
	{
		return;
	}

	servicelocator<Logger>::get_serviceLocator().LogWarning({ "Loading scene: " + std::to_string(sceneID) });
	if (m_pActiveScene)
	{
		m_pActiveScene->UnloadScene();
	}
	
	m_pActiveScene = m_scenes[sceneID].get();
	
	m_pActiveScene->LoadScene();
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
