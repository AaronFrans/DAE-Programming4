#include "Scene.h"
#include "Engine/GameObject.h"
#include "Components/Collision/CollisionComponent.h"
#include "Events/EventManager.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name)
{
	Event destroyed = Event{  };
	destroyed.eventType = "GameObject Destroyed";
	auto boundGameObjectDestroyed = std::bind(&Scene::HandleDestroyedEvent, this, std::placeholders::_1);
	EventManager::GetInstance().AddObserver(destroyed, boundGameObjectDestroyed);
}

void dae::Scene::SortGameObjectUpdate()
{

	if (!m_WasGameObjectAdded)
		return;

	std::sort(m_Objects.begin(), m_Objects.end(),
		[](const std::shared_ptr<GameObject>& pObject1, const std::shared_ptr<GameObject>& pObject2)
		{
			return pObject1->GetDrawDepth() < pObject2->GetDrawDepth();
		}
	);

	m_WasGameObjectAdded = false;
}

void dae::Scene::NormalUpdate()
{
	for (int i{ 0 }; i < m_Objects.size(); i++)
	{
		if (!m_Objects[i]->IsDestroyed())
		{
			m_Objects[i]->Update();
		}
	}
}

void dae::Scene::CleanUpdate()
{

	if (!m_WasObjectDestroyed)
		return;

	m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(),
		[&](std::shared_ptr<GameObject>& object) {
			return object->IsDestroyed();
		})
		, m_Objects.end());

	m_WasObjectDestroyed = false;
}

void dae::Scene::HandleDestroyedEvent(const Event* e)
{

	if (strcmp(e->eventType, "GameObject Destroyed") == 0)
	{
		m_WasObjectDestroyed = true;
	}
}

Scene::~Scene()
{
	m_Objects.clear();
	m_ObjectCollisions.clear();
};

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_Objects.emplace_back(std::move(object));

	m_WasGameObjectAdded = true;
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Scene::Update()
{

	//ZDepthOrder
	SortGameObjectUpdate();

	//Normal Update
	NormalUpdate();

	//Object Removed
	CleanUpdate();

	//Collision Checks
	for (auto& objectCollision : m_ObjectCollisions)
	{
		objectCollision->IsOverlappingOtherCollision(m_ObjectCollisions);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void dae::Scene::AddCollision(CollisionComponent* collision)
{
	m_ObjectCollisions.emplace_back(collision);
}

void dae::Scene::RemoveCollision(CollisionComponent* collision)
{
	m_ObjectCollisions.erase(std::remove(m_ObjectCollisions.begin(), m_ObjectCollisions.end(), collision));
}

const std::string& dae::Scene::GetName() const
{
	return m_Name;
}


