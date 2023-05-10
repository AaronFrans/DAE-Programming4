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

#include "Components/Data/HealthComponent.h"
#include "Components/Data/EarnPointsComponent.h"

#include "Components/Observers/LivesLeftComponent.h"
#include "Components/Observers/PlayerScoreComponent.h"

#include "Input/XboxController.h"
#include "Commands/MoveCommand.h"
#include "Commands/TakeDamageCommand.h"
#include "Commands/EarnPointsCommand.h"
#include "Commands/RestrictedMovementCommand.h"
#include "Commands/AttackCommand.h"

//temp, look into changing this
constexpr unsigned g_WindowWidth{ 640 }, WindowHeight{ 480 };


dae::GameObject* AddPlayerGO(dae::Scene& scene, dae::InputManager& input, unsigned playerIndex, bool useKeyboard = false)
{


	auto player = std::make_shared<dae::GameObject>(1);
	player->Init();
	scene.Add(player);
	auto playerImage = player->AddComponent<dae::ImageComponent>();

	playerImage->SetTexture("Images\\Player_Ship.png");

	const float imageWidth = playerImage->GetTextureWidth();

	const glm::vec3 playerStartPos{ g_WindowWidth / 2.f - imageWidth / 2.f, WindowHeight - 80 ,0 };
	player->GetTransform()->SetLocalPosition(playerStartPos);




	player->AddComponent<dae::ImageRenderComponent>();

	auto playerAttack = player->AddComponent<dae::AttackComponent>();

	playerAttack->SetScene(scene.GetName());

	constexpr float playerMoveSpeed{ 100 };
	constexpr glm::vec3 playerMoveDirection{ 1,0,0 };


	constexpr float playerMoveDistance{ (g_WindowWidth - 80) / 2.0f };
	const glm::vec3 playerMinPoint{ playerStartPos - glm::vec3{playerMoveDistance,0,0} };
	const glm::vec3 playerMaxPoint{ playerStartPos + glm::vec3{playerMoveDistance,0,0} };

	//TODO: Implement keyboard controlls
	if (useKeyboard)
	{
		auto pMoveCommand = input.AddKeyboardCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed });

		pMoveCommand->SetDirection(playerMoveDirection);

		pMoveCommand->SetMinPoint(playerMinPoint);
		pMoveCommand->SetMaxPoint(playerMaxPoint);

		pMoveCommand->SetMoveSpeed(-playerMoveSpeed);

		pMoveCommand = input.AddKeyboardCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed });

		pMoveCommand->SetDirection(playerMoveDirection);

		pMoveCommand->SetMinPoint(playerMinPoint);
		pMoveCommand->SetMaxPoint(playerMaxPoint);

		pMoveCommand->SetMoveSpeed(playerMoveSpeed);

		input.AddKeyboardCommand<dae::AttackCommand>(std::make_unique<dae::AttackCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_F, dae::ButtonState::Up });
	}

	auto pMoveCommand = input.AddXboxCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
		dae::XboxControllerInput{ playerIndex, dae::XboxController::ControllerButton::DPadLeft, dae::ButtonState::Pressed });

	pMoveCommand->SetDirection(playerMoveDirection);

	pMoveCommand->SetMinPoint(playerMinPoint);
	pMoveCommand->SetMaxPoint(playerMaxPoint);

	pMoveCommand->SetMoveSpeed(-playerMoveSpeed);

	pMoveCommand = input.AddXboxCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
		dae::XboxControllerInput{ playerIndex, dae::XboxController::ControllerButton::DPadRight, dae::ButtonState::Pressed });

	pMoveCommand->SetDirection(playerMoveDirection);

	pMoveCommand->SetMinPoint(playerMinPoint);
	pMoveCommand->SetMaxPoint(playerMaxPoint);

	pMoveCommand->SetMoveSpeed(playerMoveSpeed);

	input.AddXboxCommand<dae::AttackCommand>(std::make_unique<dae::AttackCommand>(player.get()),
		dae::XboxControllerInput{ playerIndex, dae::XboxController::ControllerButton::ButtonX, dae::ButtonState::Up });

	return player.get();
}

void LoadGame()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level 0");
	auto& inputManager = dae::InputManager::GetInstance();


	auto background = std::make_shared<dae::GameObject>(-1);
	background->Init();

	scene.Add(background);
	background->AddComponent<dae::ImageComponent>()->SetTexture("Images\\Background.png");
	background->AddComponent<dae::ImageRenderComponent>();


	AddPlayerGO(scene, inputManager, 0, true);




}

void load()
{
	//Rotators();

	//Input();
	LoadGame();

}

int main(int, char* []) {



	dae::Minigin engine("../Data/", g_WindowWidth, WindowHeight);
	engine.Run(load);

	return 0;
}