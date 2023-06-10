#pragma once
#include "Components/Component.h"

namespace dae
{
	struct Event;
	class ButtonComponent;
	class DetailsComponent;
	class MainMenuComponent : public Component
	{
	public:
		MainMenuComponent(GameObject* owner);
		virtual ~MainMenuComponent() = default;
		MainMenuComponent(const MainMenuComponent& other) = delete;
		MainMenuComponent(MainMenuComponent&& other) = delete;
		MainMenuComponent& operator=(const MainMenuComponent& other) = delete;
		MainMenuComponent& operator=(MainMenuComponent&& other) = delete;

		void SwitchButton(bool nextButton);
		void SelectButton();

		void LoadLevels(dae::DetailsComponent* details);
	private:


		void ResetLevels(const Event* e);
		void LoadLevelFromFile(const std::string& sceneName, const std::string& filePath,
			const std::string& nextScene, dae::DetailsComponent* details);

		int m_ActiveButton{ 0 };

		std::vector<std::string> m_LevelNames{};
		std::vector<ButtonComponent*> m_pButtons{};

		DetailsComponent* m_pDetails{};
	};

}


