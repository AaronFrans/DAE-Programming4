#include <SDL.h>
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>

#include <Engine/Minigin.h>
#include "Scene/Scene.h"

#include "Scene/SceneManager.h"
#include "Input/InputManager.h"
#include "Rendering/ResourceManager.h"
#include "Rendering/Renderer.h"
#include "Events/EventManager.h"

#include "Engine/GameObject.h"

#include "Components/FpsCounterComponent.h"
#include "Components/TextRendererComponent.h"
#include "Components/ImageRenderComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TextComponent.h"
#include "Components/ImageComponent.h"

#include "Components/Updating/RotatorComponent.h"
#include "Components/Updating/AttackComponent.h"

#include "Components/Rendering/TrashTheCacheComponent.h"
#include "Components/Rendering/TutorialComponent.h"
#include "Components/Rendering/DetailsComponent.h"
#include "Components/Rendering/MainMenuComponent.h"

#include "Components/Data/HealthComponent.h"
#include "Components/Data/EarnPointsComponent.h"

#include "Components/Observers/LivesLeftComponent.h"
#include "Components/Observers/PlayerScoreComponent.h"


#include "Components/Game/GameOverseerComponent.h"

#include "Components/Collision/CollisionComponent.h"

#include "Components/Enemies/BeeComponent.h"
#include "Components/Enemies/ButterflyComponent.h"
#include "Components/Enemies/BossComponent.h"
#include "Components/Enemies/EnemyControllerComponent.h"

#include "Components/HighScores/HighScoreComponent.h"

#include "Input/XboxController.h"

#include "Commands/MoveCommand.h"
#include "Commands/TakeDamageCommand.h"
#include "Commands/EarnPointsCommand.h"
#include "Commands/RestrictedMovementCommand.h"
#include "Commands/AttackCommand.h"
#include "Commands/ConfirmCommand.h"
#include "Commands/StartNextLevelCommand.h"
#include <Commands/SwitchButtonCommand.h>
#include <Commands/BackToMainMenuCommand.h>

#include <Sounds/SoundManager.h>
#include <Sounds/SDLSoundSystem.h>

#include <FileReading/FileReader.h>

#include <FactoryFunctions/Factories.h>


#ifdef _DEBUG
void CollisionTestFunc(const dae::CollisionData& collisionOwner, const dae::CollisionData& hitObject)
{
	std::cout << "Owner: " << collisionOwner.ownerType << " has been hit by: " << hitObject.ownerType << '\n';
}
#endif

#pragma region Sounds


void LoadSounds(dae::SoundSystem* sounds)
{
	sounds->HandleSoundData(dae::SoundData{ 0, 0, dae::SoundData::SoundType::Music, "/Sounds/BackgroundTrack.mp3", true });

	sounds->HandleSoundData(dae::SoundData{ 1, 0, dae::SoundData::SoundType::SoundEffect, "/Sounds/PlayerShoot.mp3", true });
}

#pragma endregion


#pragma region Scenes

dae::DetailsComponent* LoadDetailsScene()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Details");
	auto sceneName = scene.GetName();

	auto& inputManager = dae::InputManager::GetInstance();
	auto background = std::make_shared<dae::GameObject>(-1);
	background->Init();

	scene.Add(background);
	background->AddComponent<dae::ImageComponent>()->SetTexture("Images\\Background.png");
	background->AddComponent<dae::ImageRenderComponent>();

	auto details = std::make_shared<dae::GameObject>();
	details->Init();

	details->GetTransform()->SetLocalPosition({ 180,130,0 });

	auto detailsComp = details->AddComponent<dae::DetailsComponent>();

	scene.Add(details);

	inputManager.AddKeyboardCommand<dae::StartNextLevelCommand>(std::make_unique<dae::StartNextLevelCommand>(details.get()),
		dae::KeyboardInput{ SDL_SCANCODE_SPACE, dae::ButtonState::Up, sceneName });

	inputManager.AddXboxCommand<dae::StartNextLevelCommand>(std::make_unique<dae::StartNextLevelCommand>(details.get()),
		dae::XboxControllerInput{ 0, dae::XboxController::ControllerButton::ButtonA, dae::ButtonState::Up, sceneName });

	return detailsComp.get();
}

void LoadTestLevelScene()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level 0");
	auto& inputManager = dae::InputManager::GetInstance();


	auto background = std::make_shared<dae::GameObject>(-1);
	background->Init();

	scene.Add(background);
	background->AddComponent<dae::ImageComponent>()->SetTexture("Images\\Background.png");
	background->AddComponent<dae::ImageRenderComponent>();


	auto overseer = std::make_shared<dae::GameObject>();
	overseer->Init();

	scene.Add(overseer);

	overseer->AddComponent<dae::GameOverseerComponent>();

	auto pPlayer = AddPlayerGO(scene, inputManager, 0, true);
	auto pPlayerTransform = pPlayer->GetTransform().get();
	std::vector<dae::EnemyControllerComponent*> butterflyControllers{};

	dae::AddEnemyGO(scene, pPlayerTransform, glm::vec3{ 100, 50, 0 }, dae::EnemyTypes::Butterfly, butterflyControllers);

	dae::AddEnemyGO(scene, pPlayerTransform, glm::vec3{ 50, 50, 0 }, dae::EnemyTypes::Boss, butterflyControllers);



}

void LoadHighScoreScene()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Highscores");
	auto sceneName = scene.GetName();

	auto& inputManager = dae::InputManager::GetInstance();

	auto background = std::make_shared<dae::GameObject>(-1);
	background->Init();

	scene.Add(background);
	background->AddComponent<dae::ImageComponent>()->SetTexture("Images\\Background.png");
	background->AddComponent<dae::ImageRenderComponent>();

	auto highscores = std::make_shared<dae::GameObject>(0);
	highscores->Init();
	scene.Add(highscores);

	auto pText = highscores->AddComponent<dae::TextComponent>();
	pText->SetText("HighScores: ");
	pText->SetColor({ 255, 255, 30 });

	auto font = dae::ResourceManager::GetInstance().LoadFont("Galaga.ttf", 15);
	pText->SetFont(font);

	highscores->AddComponent<dae::TextRendererComponent>();

	highscores->AddComponent<dae::HighScoreComponent>();
	highscores->GetTransform()->SetLocalPosition({ 230, 150 ,0 });

	inputManager.AddKeyboardCommand<dae::BackToMainMenuCommand>(std::make_unique<dae::BackToMainMenuCommand>(highscores.get()),
		dae::KeyboardInput{ SDL_SCANCODE_SPACE, dae::ButtonState::Up, sceneName });

	inputManager.AddXboxCommand<dae::BackToMainMenuCommand>(std::make_unique<dae::BackToMainMenuCommand>(highscores.get()),
		dae::XboxControllerInput{ 0, dae::XboxController::ControllerButton::ButtonA, dae::ButtonState::Up, sceneName });
}

dae::MainMenuComponent* LoadMainMenuScene(dae::SoundSystem* sounds)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("MainMenu");
	auto& inputManager = dae::InputManager::GetInstance();


	auto background = std::make_shared<dae::GameObject>(-1);
	background->Init();

	scene.Add(background);
	background->AddComponent<dae::ImageComponent>()->SetTexture("Images\\Background.png");
	background->AddComponent<dae::ImageRenderComponent>();



	auto mainMenu = std::make_shared<dae::GameObject>(0);
	mainMenu->Init();
	scene.Add(mainMenu);
	auto mainMenuComp = mainMenu->AddComponent<dae::MainMenuComponent>();
	mainMenu->GetTransform()->SetLocalPosition({ 180,130,0 });

	auto sceneName = scene.GetName();

	inputManager.AddKeyboardCommand<dae::ConfirmCommand>(std::make_unique<dae::ConfirmCommand>(mainMenu.get()),
		dae::KeyboardInput{ SDL_SCANCODE_SPACE, dae::ButtonState::Up, sceneName });


	inputManager.AddXboxCommand<dae::ConfirmCommand>(std::make_unique<dae::ConfirmCommand>(mainMenu.get()),
		dae::XboxControllerInput{ 0, dae::XboxController::ControllerButton::ButtonA, dae::ButtonState::Up, sceneName });

	inputManager.AddKeyboardCommand<dae::SwitchButtonCommand>(std::make_unique<dae::SwitchButtonCommand>(mainMenu.get()),
		dae::KeyboardInput{ SDL_SCANCODE_RIGHT, dae::ButtonState::Up, sceneName })->SetMoveNext(true);

	inputManager.AddKeyboardCommand<dae::SwitchButtonCommand>(std::make_unique<dae::SwitchButtonCommand>(mainMenu.get()),
		dae::KeyboardInput{ SDL_SCANCODE_LEFT, dae::ButtonState::Up, sceneName });

	inputManager.AddXboxCommand<dae::SwitchButtonCommand>(std::make_unique<dae::SwitchButtonCommand>(mainMenu.get()),
		dae::XboxControllerInput{ 0, dae::XboxController::ControllerButton::DPadLeft, dae::ButtonState::Up, sceneName });

	inputManager.AddXboxCommand<dae::SwitchButtonCommand>(std::make_unique<dae::SwitchButtonCommand>(mainMenu.get()),
		dae::XboxControllerInput{ 0, dae::XboxController::ControllerButton::DPadRight, dae::ButtonState::Up, sceneName })->SetMoveNext(true);

	sounds->HandleSoundData(dae::SoundData{ 0, 0.1f, dae::SoundData::SoundType::Music });

	return mainMenuComp.get();


}

#pragma endregion


void LoadGame()
{
	auto ss = dae::SoundManager::GetInstance().GetSoundSystem();
	LoadSounds(ss);

	auto detailComp = LoadDetailsScene();
	LoadHighScoreScene();

	auto mmComp = LoadMainMenuScene(ss);

	mmComp->LoadLevels(detailComp);

	dae::SceneManager::GetInstance().SetActiveScene("MainMenu");

}

void load()
{

	std::srand(static_cast<unsigned int>(std::time(0)));

	//Rotators();

	//Input();
	LoadGame();

}

int main(int, char* []) {

	std::string filePath = "../Data/";
	dae::FileReader::GetInstance().Init(filePath);

	dae::Minigin engine(filePath, dae::g_WindowWidth, dae::g_WindowHeight);
	engine.Run(load);

	return 0;
}