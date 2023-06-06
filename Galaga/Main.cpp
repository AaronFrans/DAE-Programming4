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

#include "Components/Collision/CollisionComponent.h"

#include "Components/Enemies/BeeComponent.h"
#include "Components/Enemies/EnemyControllerComponent.h"

#include "Input/XboxController.h"
#include "Commands/MoveCommand.h"
#include "Commands/TakeDamageCommand.h"
#include "Commands/EarnPointsCommand.h"
#include "Commands/RestrictedMovementCommand.h"
#include "Commands/AttackCommand.h"

#include <Sounds/SoundManager.h>
#include <Sounds/SDLSoundSystem.h>

//TODO: look into changing this to not be global
constexpr unsigned g_WindowWidth{ 640 }, WindowHeight{ 480 };


#ifdef _DEBUG
void CollisionTestFunc(const dae::CollisionData& collisionOwner, const dae::CollisionData& hitObject)
{
	std::cout << "Owner: " << collisionOwner.ownerType << " has been hit by: " << hitObject.ownerType << '\n';
}
#endif


enum class EnemyTypes
{
	Bee,
	Butterfly,
	Boss
};

dae::GameObject* AddPlayerGO(dae::Scene& scene, dae::InputManager& input, unsigned playerIndex, bool useKeyboard = false)
{


	auto player = std::make_shared<dae::GameObject>(1);
	player->Init();
	scene.Add(player);
	auto playerImage = player->AddComponent<dae::ImageComponent>();

	playerImage->SetTexture("Images\\Player_Ship.png");

	const float imageWidth = playerImage->GetTextureWidth();
	const float imageHeight = playerImage->GetTextureHeight();

	const glm::vec3 playerStartPos{ g_WindowWidth / 2.f - imageWidth / 2.f, WindowHeight - 80, 0 };
	player->GetTransform()->SetLocalPosition(playerStartPos);


	player->AddComponent<dae::ImageRenderComponent>();

	auto playerAttack = player->AddComponent<dae::AttackComponent>();

	playerAttack->SetScene(scene.GetName());

	constexpr float playerMoveSpeed{ 100 };
	constexpr glm::vec3 playerMoveDirection{ 1, 0, 0 };


	constexpr float playerMoveDistance{ (g_WindowWidth - 80) / 2.0f };
	const glm::vec3 playerMinPoint{ playerStartPos - glm::vec3{playerMoveDistance, 0, 0} };
	const glm::vec3 playerMaxPoint{ playerStartPos + glm::vec3{playerMoveDistance, 0, 0} };

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


	auto collision = player->AddComponent<dae::CollisionComponent>();
	scene.AddCollision(collision.get());
	collision->SetCollisionData({ "Player", player.get() });

	float collisionWidth{ imageWidth }, collisionHeight{ imageHeight };
	collision->SetBounds(collisionWidth, collisionHeight);

	collision->SetCallback(CollisionTestFunc);
	//collision->EnableDebugSquare();

	collision->SetScene(&scene);

	return player.get();
}

void AddEnemyTexture(std::shared_ptr<dae::GameObject> enemy, const std::string& texturePath)
{
	auto playerImage = enemy->AddComponent<dae::ImageComponent>();

	playerImage->SetTexture(texturePath);
	enemy->AddComponent<dae::ImageRenderComponent>();
}

dae::GameObject* AddEnemyGO(dae::Scene& scene, glm::vec3 pos, EnemyTypes enemyType)
{
	auto enemy = std::make_shared<dae::GameObject>(0);
	enemy->Init();
	scene.Add(enemy);

	enemy->GetTransform()->SetLocalPosition(pos);


	switch (enemyType)
	{
	case EnemyTypes::Bee:
	{
		AddEnemyTexture(enemy, "Images\\Galaga_Bee.png");
		enemy->AddComponent<dae::BeeComponent>()->SetMaxYPos(200);
	}
	break;
	case EnemyTypes::Butterfly:
		AddEnemyTexture(enemy, "Images\\Galaga_Butterfly.png");
		break;
	case EnemyTypes::Boss:
		AddEnemyTexture(enemy, "Images\\Galaga_Boss.png");
		break;
	default:
		break;
	}


	auto attacking = enemy->GetComponent<dae::BaseEnemyComponent>();
	attacking->SetFormationPosition(pos);

	enemy->AddComponent<dae::EnemyControllerComponent>();

	auto playerImage = enemy->GetComponent<dae::ImageComponent>();

	const float imageWidth = playerImage->GetTextureWidth(),
		imageHeight = playerImage->GetTextureHeight();

	auto collision = enemy->AddComponent<dae::CollisionComponent>();
	scene.AddCollision(collision.get());
	collision->SetCollisionData({ "Enemy", enemy.get() });

	float collisionWidth{ imageWidth }, collisionHeight{ imageHeight };
	collision->SetBounds(collisionWidth, collisionHeight);
	//collision->EnableDebugSquare();

	auto boundHitCallback = std::bind(&dae::BaseEnemyComponent::OnHitCallback, attacking, std::placeholders::_1, std::placeholders::_2);
	collision->SetCallback(boundHitCallback);

	collision->SetScene(&scene);

	return enemy.get();
}

void LoadSounds(dae::SoundSystem* sounds)
{
	sounds->NotifySound(dae::SoundData{ 0, 0, dae::SoundData::SoundType::Music, "/Sounds/BackgroundTrack.mp3", true });

	sounds->NotifySound(dae::SoundData{ 1, 0, dae::SoundData::SoundType::SoundEffect, "/Sounds/PlayerShoot.mp3", true });
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


	AddEnemyGO(scene, glm::vec3{ 50, 50, 0 }, EnemyTypes::Bee);
	auto tutorial = std::make_shared<dae::GameObject>();
	tutorial->Init();
	scene.Add(tutorial);

	tutorial->AddComponent<dae::TutorialComponent>();

	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();
	LoadSounds(sounds);

	sounds->NotifySound(dae::SoundData{ 0, 0.1f, dae::SoundData::SoundType::Music });

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