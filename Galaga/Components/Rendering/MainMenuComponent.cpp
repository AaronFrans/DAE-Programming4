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

	pSingleButton->SetOnClick([]() {
		SceneManager::GetInstance().SetActiveScene("Level1");
		});

	singlePlayerButtonGO->SetParent(owner, false);

	m_pButtons.emplace_back(pSingleButton.get());


	auto coopButtonGO = std::make_shared<GameObject>();
	coopButtonGO->Init();
	auto pCoopButton = coopButtonGO->AddComponent<ButtonComponent>();
	pCoopButton->SetButtonText("CO-OP");

	pCoopButton->SetOnClick([]() {
		SceneManager::GetInstance().SetActiveScene("Level1");
		});

	coopButtonGO->SetParent(owner, false);

	coopButtonGO->GetTransform()->SetLocalPosition({ 0, buttonHeightOffset * 1, 0 });
	m_pButtons.emplace_back(pCoopButton.get());



	auto versusButtonGO = std::make_shared<GameObject>();
	versusButtonGO->Init();
	auto pVersusButton = versusButtonGO->AddComponent<ButtonComponent>();
	pVersusButton->SetButtonText("Versus");

	pVersusButton->SetOnClick([]() {
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

void dae::MainMenuComponent::LoadLevels(dae::DetailsComponent* pDetails)
{

	m_pDetails = pDetails;

	LoadLevelFromFile("Level1", "JsonFiles/Level1.json", "Level2", pDetails);
	LoadLevelFromFile("Level2", "JsonFiles/Level2.json", "Level3", pDetails);
	LoadLevelFromFile("Level3", "JsonFiles/Level3.json", "Highscores", pDetails);
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
	LoadLevelFromFile("Level1", "JsonFiles/Level1.json", "Level2", m_pDetails);
	LoadLevelFromFile("Level2", "JsonFiles/Level2.json", "Level3", m_pDetails);
	LoadLevelFromFile("Level3", "JsonFiles/Level3.json", "Highscores", m_pDetails);


	sceneManager.SetActiveScene("MainMenu");

}

void dae::MainMenuComponent::LoadLevelFromFile(const std::string& sceneName, const std::string& filePath,
	const std::string& nextScene, dae::DetailsComponent* details)
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

	auto pPlayer = AddPlayerGO(scene, inputManager, 0, true);
	auto pPlayerTransform = pPlayer->GetTransform().get();


	//LoadEnemiePosFromFile
	std::vector<dae::EnemyControllerComponent*> butterflyControllers{};
	auto fileEnemies = dae::FileReader::GetInstance().ReadEnemies(filePath);
	for (auto& enemy : fileEnemies)
	{
		dae::AddEnemyGO(scene, pPlayerTransform, glm::vec3{ enemy.x, enemy.y, 0 }, static_cast<dae::EnemyTypes>(enemy.enemyType), butterflyControllers);
	}

	dae::LoadUI(scene, nextScene);

	auto tutorial = std::make_shared<dae::GameObject>();
	tutorial->Init();
	scene.Add(tutorial);

	tutorial->AddComponent<dae::TutorialComponent>();

	m_LevelNames.push_back(sceneName);
}
