#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	m_ActiveScene->Update();
}

void dae::SceneManager::Render()
{
	m_ActiveScene->Render();
}

void dae::SceneManager::Quit()
{
	m_Scenes.clear();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);

	if (m_ActiveScene)
		m_ActiveScene->SetActive(false);

	m_ActiveScene = scene;
	m_ActiveScene->SetActive(true);

	return *scene;
}

dae::Scene& dae::SceneManager::GetSceneByName(const std::string& name)
{
	for (auto& scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			return *scene;
		}
	}

	return CreateScene(name);
}

dae::Scene& dae::SceneManager::GetActiveScene()
{
	return *m_ActiveScene;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (auto& scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			m_ActiveScene->SetActive(false);
			m_ActiveScene = scene;
			m_ActiveScene->SetActive(true);

			return;
		}
	}
}

bool dae::SceneManager::IsSceneActive(const std::string& name)
{
	return m_ActiveScene->GetName() == name;
}
