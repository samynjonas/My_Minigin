#include "Scene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "CollisionManager.h"

#include "RenderComponent.h"
#include "RenderingManager.h"

#include "CameraComponent.h"

#include <iostream>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) 
{

}

void Scene::Add(std::unique_ptr<GameObject> object)
{
	if (this == SceneManager::GetInstance().GetActiveScene())
	{
		object->SetActiveState(true);
	}

	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
	if (object != nullptr)
	{
		object->MarkForDead();
	}

	//InputManager::GetInstance().UnbindCommands();
	CollisionManager::GetInstance().CheckForDeadColliders();
	RenderingManager::GetInstance().CheckForDead();
	

	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	for (auto& object : m_objects)
	{
		if (object == nullptr)
		{
			continue;
		}
		object->MarkForDead();
	}

	InputManager::GetInstance().UnbindCommands();
	//CollisionManager::GetInstance().CheckForDeadColliders();

	m_objects.clear();
}

void Scene::Update()
{
	// Mark objects for removal
	for (auto& object : m_objects)
	{
		if (object != nullptr && object->IsMarkedForDead())
		{
			object->MarkForDead();
		}
	}

	// Remove marked objects
	m_objects.erase(
		std::remove_if(m_objects.begin(), m_objects.end(),
			[](const std::unique_ptr<dae::GameObject>& obj) 
			{
				return obj != nullptr && obj->IsMarkedForDead();
			}),
		m_objects.end()
	);

	// Update remaining objects
	for (const auto& object : m_objects)
	{
		if (object && !object->IsMarkedForDead())
		{
			object->Update();
		}
	}

	for (const auto& object : m_objects)
	{
		if (object && !object->IsMarkedForDead())
		{
			object->LateUpdate();
		}
	}
}

void Scene::Render() const
{
	//RenderingManager::GetInstance().Render();
}

void Scene::LoadScene()
{
	NotifyObservers(LevelLoad, this);
	ActivateAll();
}

void Scene::UnloadScene()
{
	NotifyObservers(LevelUnload, this);
	DeActivateAll();
}

void Scene::ActivateAll()
{
	for (auto& object : m_objects)
	{
		if (object != nullptr)
		{
			object->SetActiveState(true);
		
			auto camera = object->GetComponent<CameraComponent>();
			if (camera)
			{
				RenderingManager::GetInstance().SetActiveCamera(camera);
			}
		
		}
	}
}

void Scene::DeActivateAll()
{
	for (auto& object : m_objects)
	{
		if (object != nullptr)
		{
			object->SetActiveState(false);
		}
	}
}

void Scene::Cleanup()
{
	//for (auto& object : m_objects)
	//{
	//	if (object->IsMarkedForDead())
	//	{
	//		m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
	//	}
	//}
}
