#include "Scene.h"
#include "Engine/GameObject.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

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
	if (m_WasGameObjectAdded)
	{
		std::sort(m_Objects.begin(), m_Objects.end(),
			[](const std::shared_ptr<GameObject>& pObject1, const std::shared_ptr<GameObject>& pObject2)
			{
				return pObject1->GetDrawDepth() < pObject2->GetDrawDepth();
			}
		);

		m_WasGameObjectAdded = false;
	}

	for (auto& object : m_Objects)
	{
		if (!object->IsDestroyed())
			object->Update();
	}

	m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(),
		[&](std::shared_ptr<GameObject>& object) {
			return object->IsDestroyed();
		})
		, m_Objects.end());

}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

const std::string& dae::Scene::GetName() const
{
	return m_Name;
}


