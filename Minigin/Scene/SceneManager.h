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

		void Update();
		void Render();

		void Quit();

		Scene& GetSceneByName(const std::string& name);
		Scene& GetActiveScene();

		void SetActiveScene(const std::string& name);
		bool IsSceneActive(const std::string& name);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::shared_ptr<Scene> m_ActiveScene;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
	};
}
