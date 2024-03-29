#include "SceneManager.h"
#include "Scene.h"

#include "Input/InputManager.h"

#include "Commands/MuteCommand.h"

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
	m_ActiveScene = nullptr;
}


dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);

	if (m_ActiveScene)
		m_ActiveScene->SetActive(false);

	m_ActiveScene = scene;
	m_ActiveScene->SetActive(true);



	InputManager::GetInstance().AddKeyboardCommand<dae::MuteCommand>(std::make_unique<dae::MuteCommand>(),
		dae::KeyboardInput{ SDL_SCANCODE_F1, dae::ButtonState::Up, name });

	return *scene;
}


void  dae::SceneManager::RemoveScene(const std::string& name)
{
	m_Scenes.erase(std::remove_if(m_Scenes.begin(), m_Scenes.end(), [&](std::shared_ptr<Scene> pScene) {
		return pScene->GetName() == name;
		}));
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
