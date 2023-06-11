#include "MainMenuComponent.h"

#include "FactoryFunctions/Factories.h"

#include <Scene/Scene.h>
#include <Scene/SceneManager.h>

#include <Events/Event.h>
#include <Events/EventManager.h>

#include <Input/InputManager.h>

#include "Components/TransformComponent.h"
#include <Components/Game/GameOverseerComponent.h>
#include <Components/ImageRenderComponent.h>
#include <Components/ImageComponent.h>
#include "ButtonComponent.h"
#include "DetailsComponent.h"
#include "TutorialComponent.h"

#include "Commands/SkipLevelCommand.h"

#include <FileReading/FileReader.h>

dae::MainMenuComponent::MainMenuComponent(GameObject* owner)
	:Component(owner)
{
	constexpr int buttonHeightOffset{ 50 };
	auto singlePlayerButtonGO = std::make_shared<GameObject>();
	singlePlayerButtonGO->Init();
	auto pSingleButton = singlePlayerButtonGO->AddComponent<ButtonComponent>();
	pSingleButton->SetButtonText("Single Player");
	pSingleButton->SetActive(true);

	pSingleButton->SetOnClick([&]() {
		LoadLevels();
		SceneManager::GetInstance().SetActiveScene("Level1");
		});

	singlePlayerButtonGO->SetParent(owner, false);

	m_pButtons.emplace_back(pSingleButton.get());


	auto coopButtonGO = std::make_shared<GameObject>();
	coopButtonGO->Init();
	auto pCoopButton = coopButtonGO->AddComponent<ButtonComponent>();
	pCoopButton->SetButtonText("CO-OP");

	pCoopButton->SetOnClick([&]() {
		LoadMultiplayerLevels();
		SceneManager::GetInstance().SetActiveScene("Level1");
		});

	coopButtonGO->SetParent(owner, false);

	coopButtonGO->GetTransform()->SetLocalPosition({ 0, buttonHeightOffset * 1, 0 });
	m_pButtons.emplace_back(pCoopButton.get());



	auto versusButtonGO = std::make_shared<GameObject>();
	versusButtonGO->Init();
	auto pVersusButton = versusButtonGO->AddComponent<ButtonComponent>();
	pVersusButton->SetButtonText("Versus");

	pVersusButton->SetOnClick([&]() {
		LoadVersusLevels();
		SceneManager::GetInstance().SetActiveScene("Level1");
		});

	versusButtonGO->SetParent(owner, false);

	versusButtonGO->GetTransform()->SetLocalPosition({ 0, buttonHeightOffset * 2, 0 });
	m_pButtons.emplace_back(pVersusButton.get());


	auto boundReset = std::bind(&MainMenuComponent::ResetLevels, this, std::placeholders::_1);
	Event resetEvent;
	resetEvent.eventType = "Reset";
	EventManager::GetInstance().AddObserver(resetEvent, boundReset);



}

void dae::MainMenuComponent::SwitchButton(bool nextButton)
{

	m_pButtons[m_ActiveButton]->SetActive(false);
	if (nextButton)
	{
		++m_ActiveButton;
		if (m_ActiveButton == static_cast<int>(m_pButtons.size()))
			m_ActiveButton = 0;
	}
	else
	{
		--m_ActiveButton;
		if (m_ActiveButton < 0)
			m_ActiveButton = static_cast<int>(m_pButtons.size() - 1);
	}
	m_pButtons[m_ActiveButton]->SetActive(true);


}

void dae::MainMenuComponent::SelectButton()
{
	m_pButtons[m_ActiveButton]->DoClick();
}

void dae::MainMenuComponent::SetDetailsLevels(dae::DetailsComponent* pDetails)
{

	m_pDetails = pDetails;
}

void dae::MainMenuComponent::LoadLevels()
{

	LoadLevelFromFile("Level1", "JsonFiles/Level1.json", "Level2", m_pDetails, GameMode::SinglePlayer);
	LoadLevelFromFile("Level2", "JsonFiles/Level2.json", "Level3", m_pDetails, GameMode::SinglePlayer);
	LoadLevelFromFile("Level3", "JsonFiles/Level3.json", "Highscores", m_pDetails, GameMode::SinglePlayer);


	SceneManager::GetInstance().SetActiveScene("MainMenu");
}

void dae::MainMenuComponent::LoadMultiplayerLevels()
{
	LoadLevelFromFile("Level1", "JsonFiles/Level1.json", "Level2", m_pDetails, GameMode::COOP);
	LoadLevelFromFile("Level2", "JsonFiles/Level2.json", "Level3", m_pDetails, GameMode::COOP);
	LoadLevelFromFile("Level3", "JsonFiles/Level3.json", "Highscores", m_pDetails, GameMode::COOP);


	SceneManager::GetInstance().SetActiveScene("MainMenu");
}

void dae::MainMenuComponent::LoadVersusLevels()
{
	LoadLevelFromFile("Level1", "JsonFiles/Level1.json", "Level2", m_pDetails, GameMode::Versus);
	LoadLevelFromFile("Level2", "JsonFiles/Level2.json", "Level3", m_pDetails, GameMode::Versus);
	LoadLevelFromFile("Level3", "JsonFiles/Level3.json", "Highscores", m_pDetails, GameMode::Versus);


	SceneManager::GetInstance().SetActiveScene("MainMenu");
}

void dae::MainMenuComponent::ResetLevels(const Event* e)
{
	assert(m_pDetails && "Use MainMenuComponent::LoadLevels before you can use reset");

	if (!(strcmp(e->eventType, "Reset") == 0))
		return;
	auto& sceneManager = SceneManager::GetInstance();

	for (auto& name : m_LevelNames)
	{
		sceneManager.RemoveScene(name);
	}
	m_LevelNames.clear();

}

void dae::MainMenuComponent::LoadLevelFromFile(const std::string& sceneName, const std::string& filePath,
	const std::string& nextScene, dae::DetailsComponent* details, GameMode mode)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(sceneName);
	auto& inputManager = dae::InputManager::GetInstance();


	auto background = std::make_shared<dae::GameObject>(-1);
	background->Init();

	scene.Add(background);
	background->AddComponent<dae::ImageComponent>()->SetTexture("Images\\Background.png");
	background->AddComponent<dae::ImageRenderComponent>();


	auto overseer = std::make_shared<dae::GameObject>();
	overseer->Init();

	scene.Add(overseer);


	inputManager.AddKeyboardCommand<dae::SkipLevelCommand>(std::make_unique<dae::SkipLevelCommand>(overseer.get()),
		dae::KeyboardInput{ SDL_SCANCODE_F2, dae::ButtonState::Up, sceneName });

	auto pOverseerComp = overseer->AddComponent<dae::GameOverseerComponent>();
	pOverseerComp->SetSceneName(sceneName);
	pOverseerComp->SetNextSceneName(nextScene);
	pOverseerComp->SetDetails(details);

	std::vector<TransformComponent*> pPlayerTransforms{};

	switch (mode)
	{
	case dae::MainMenuComponent::GameMode::SinglePlayer: 
	{
		auto pPlayer = AddPlayerGO(scene, inputManager, 0, true, "Images/Player_Ship.png");
		pPlayerTransforms.emplace_back(pPlayer->GetTransform().get());
	}
	break;
	case dae::MainMenuComponent::GameMode::Versus:
	{
		auto pPlayer = AddPlayerGO(scene, inputManager, 1, true, "Images/Player_Ship.png");
		pPlayerTransforms.emplace_back(pPlayer->GetTransform().get());
	}
	break;
	case dae::MainMenuComponent::GameMode::COOP:
	{
		auto pPlayer = AddPlayerGO(scene, inputManager, 1, true, "Images/Player_Ship.png");
		pPlayer->GetTransform()->SetLocalPosition(pPlayer->GetTransform()->GetLocalPosition() - glm::vec3{20, 0, 0});
		pPlayerTransforms.emplace_back(pPlayer->GetTransform().get());
		auto pPlayer2 = AddPlayerGO(scene, inputManager, 0, false, "Images/Player_2_Ship.png");

		pPlayer2->GetTransform()->SetLocalPosition(pPlayer2->GetTransform()->GetLocalPosition() + glm::vec3{20, 0, 0});
		pPlayerTransforms.emplace_back(pPlayer2->GetTransform().get());
	}
	break;
	}


	std::vector<dae::EnemyControllerComponent*> butterflyControllers{};
	auto fileEnemies = dae::FileReader::GetInstance().ReadEnemies(filePath);
	for (auto& enemy : fileEnemies)
	{
		dae::AddEnemyGO(scene, pPlayerTransforms, glm::vec3{ enemy.x, enemy.y, 0 }, static_cast<dae::EnemyTypes>(enemy.enemyType), butterflyControllers);
	}

	
	if (mode == MainMenuComponent::GameMode::Versus)
	{
		dae::AddPlayerEnemyGO(scene, pPlayerTransforms, glm::vec3{ 300, 60, 0 }, butterflyControllers);
	}

	switch (mode)
	{
	case dae::MainMenuComponent::GameMode::SinglePlayer:
	case dae::MainMenuComponent::GameMode::Versus:
	{
		dae::LoadUI(scene, nextScene);
	}
	break;
	case dae::MainMenuComponent::GameMode::COOP:
		dae::LoadUI(scene, nextScene, true);
		break;
	}

	m_LevelNames.push_back(sceneName);
}
