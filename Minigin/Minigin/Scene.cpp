#include "Scene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "CollisionManager.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) 
{
	
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	if (object != nullptr)
	{
		object->MarkForDead();
	}

	//InputManager::GetInstance().UnbindCommands();
	CollisionManager::GetInstance().CheckForDeadColliders();

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

	//InputManager::GetInstance().UnbindCommands();
	//CollisionManager::GetInstance().CheckForDeadColliders();

	m_objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		if (object == nullptr)
		{
			Remove(object);
			continue;
		}	

		if (object)
		{
			//Check if object is marked for dead
			if (object->IsMarkedForDead())
			{
				Remove(object);
				continue;
			}

			object->Update();
		}
	}

	for (auto& object : m_objects)
	{
		if (object)
		{
			if (object->IsMarkedForDead())
			{
				Remove(object);
				continue;
			}

			object->LateUpdate();
		}
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		if (object)
		{
			object->Render();
		}
	}

	for (const auto& object : m_objects)
	{
		if (object)
		{
			object->LateRender();
		}
	}
}

void Scene::LoadScene()
{
	NotifyObservers(LevelLoad, this);
}

void Scene::UnloadScene()
{
	NotifyObservers(LevelUnload, this);
}

