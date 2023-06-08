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
#include <Components/Enemies/ButterflyComponent.h>
#include "Components/Enemies/BossComponent.h"
#include "Components/Enemies/EnemyControllerComponent.h"

#include "Input/XboxController.h"
#include "Commands/MoveCommand.h"
#include "Commands/TakeDamageCommand.h"
#include "Commands/EarnPointsCommand.h"
#include "Commands/RestrictedMovementCommand.h"
#include "Commands/AttackCommand.h"

#include <Sounds/SoundManager.h>
#include <Sounds/SDLSoundSystem.h>

namespace dae
{
	constexpr unsigned g_WindowWidth{ 640 }, g_WindowHeight{ 480 };

	enum class EnemyTypes
	{
		Bee,
		Butterfly,
		Boss
	};
}


#ifdef _DEBUG
void CollisionTestFunc(const dae::CollisionData& collisionOwner, const dae::CollisionData& hitObject)
{
	std::cout << "Owner: " << collisionOwner.ownerType << " has been hit by: " << hitObject.ownerType << '\n';
}
#endif



dae::GameObject* AddPlayerGO(dae::Scene& scene, dae::InputManager& input, unsigned playerIndex, bool useKeyboard = false)
{


	auto player = std::make_shared<dae::GameObject>(1);
	player->Init();
	scene.Add(player);
	auto playerImage = player->AddComponent<dae::ImageComponent>();

	playerImage->SetTexture("Images\\Player_Ship.png");

	const float imageWidth = playerImage->GetTextureWidth();
	const float imageHeight = playerImage->GetTextureHeight();

	const glm::vec3 playerStartPos{ dae::g_WindowWidth / 2.f - imageWidth / 2.f, dae::g_WindowHeight - 80, 0 };
	player->GetTransform()->SetLocalPosition(playerStartPos);


	player->AddComponent<dae::ImageRenderComponent>();

	auto playerAttack = player->AddComponent<dae::AttackComponent>();

	playerAttack->SetScene(scene.GetName());
	playerAttack->SetPlayerIndex(playerIndex);

	constexpr float playerMoveSpeed{ 100 };
	constexpr glm::vec3 playerMoveDirection{ 1, 0, 0 };


	constexpr float playerMoveDistance{ (dae::g_WindowWidth - 80) / 2.0f };
	const glm::vec3 playerMinPoint{ playerStartPos - glm::vec3{playerMoveDistance, 0, 0} };
	const glm::vec3 playerMaxPoint{ playerStartPos + glm::vec3{playerMoveDistance, 0, 0} };

	if (useKeyboard)
	{
		auto pMoveCommand = input.AddKeyboardCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed });

		pMoveCommand->SetDirection(playerMoveDirection);
		pMoveCommand->SetPlayerIndex(playerIndex);

		pMoveCommand->SetMinPoint(playerMinPoint);
		pMoveCommand->SetMaxPoint(playerMaxPoint);

		pMoveCommand->SetMoveSpeed(-playerMoveSpeed);

		pMoveCommand = input.AddKeyboardCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed });

		pMoveCommand->SetDirection(playerMoveDirection);
		pMoveCommand->SetPlayerIndex(playerIndex);

		pMoveCommand->SetMinPoint(playerMinPoint);
		pMoveCommand->SetMaxPoint(playerMaxPoint);

		pMoveCommand->SetMoveSpeed(playerMoveSpeed);

		input.AddKeyboardCommand<dae::AttackCommand>(std::make_unique<dae::AttackCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_F, dae::ButtonState::Up });
	}

	auto pMoveCommand = input.AddXboxCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
		dae::XboxControllerInput{ playerIndex, dae::XboxController::ControllerButton::DPadLeft, dae::ButtonState::Pressed });

	pMoveCommand->SetDirection(playerMoveDirection);
	pMoveCommand->SetPlayerIndex(playerIndex);

	pMoveCommand->SetMinPoint(playerMinPoint);
	pMoveCommand->SetMaxPoint(playerMaxPoint);

	pMoveCommand->SetMoveSpeed(-playerMoveSpeed);

	pMoveCommand = input.AddXboxCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
		dae::XboxControllerInput{ playerIndex, dae::XboxController::ControllerButton::DPadRight, dae::ButtonState::Pressed });

	pMoveCommand->SetDirection(playerMoveDirection);
	pMoveCommand->SetPlayerIndex(playerIndex);

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

	collision->SetCallback([&](const dae::CollisionData&, const dae::CollisionData& hitObject) {

		if (!(strcmp(hitObject.ownerType.c_str(), "EnemyAttack") == 0))
			return;

		std::unique_ptr<dae::PlayerEvent> event = std::make_unique<dae::PlayerEvent>();
		event->eventType = "PlayerDied";
		event->playerIndex = 0;
		dae::EventManager::GetInstance().SendEventMessage(std::move(event));
		});

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

dae::GameObject* AddEnemyGO(dae::Scene& scene, dae::TransformComponent* pPlayer, glm::vec3 pos,
	dae::EnemyTypes enemyType, std::vector<dae::EnemyControllerComponent*>& butterflies)
{
	auto enemy = std::make_shared<dae::GameObject>(0);
	enemy->Init();
	scene.Add(enemy);

	enemy->GetTransform()->SetLocalPosition(pos);

	int minOffset{ 0 };
	int maxOffset{ 8000 };

	int randomNum = (std::rand() % maxOffset) + minOffset;
	float randomOffset = randomNum / 1000.0f;

	switch (enemyType)
	{
	case dae::EnemyTypes::Bee:
	{
		AddEnemyTexture(enemy, "Images\\Galaga_Bee.png");
		enemy->AddComponent<dae::BeeComponent>();
		enemy->AddComponent<dae::EnemyControllerComponent>()->AddMinDelayOffset(randomOffset);
	}
	break;
	case dae::EnemyTypes::Butterfly:
	{
		AddEnemyTexture(enemy, "Images\\Galaga_Butterfly.png");
		enemy->AddComponent<dae::ButterflyComponent>();
		auto pController = enemy->AddComponent<dae::EnemyControllerComponent>();
		pController->AddMinDelayOffset(randomOffset);
		butterflies.push_back(pController.get());
	}
	break;
	case dae::EnemyTypes::Boss:
		AddEnemyTexture(enemy, "Images\\Galaga_Boss.png");
		enemy->AddComponent<dae::BossComponent>()->SetButterflies(butterflies);
		enemy->AddComponent<dae::EnemyControllerComponent>()->AddMinDelayOffset(randomOffset);
		break;
	default:
		break;
	}


	auto attacking = enemy->GetComponent<dae::BaseEnemyComponent>();
	attacking->SetFormationPosition(pos);
	attacking->SetPlayerTransform(pPlayer);
	attacking->SetScreenCenter({ dae::g_WindowWidth / 2.0f, dae::g_WindowHeight / 2.0f, 0 });
	attacking->SetScene(scene.GetName());
	attacking->SetMaxYPos(dae::g_WindowHeight - 20);

	if (auto bossComponent = enemy->GetComponent<dae::BossComponent>())
	{
		bossComponent->SetupCollision();
	}


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
	sounds->HandleSoundData(dae::SoundData{ 0, 0, dae::SoundData::SoundType::Music, "/Sounds/BackgroundTrack.mp3", true });

	sounds->HandleSoundData(dae::SoundData{ 1, 0, dae::SoundData::SoundType::SoundEffect, "/Sounds/PlayerShoot.mp3", true });
}

void LoadUI(dae::Scene& scene)
{
	auto pUI = std::make_shared<dae::GameObject>();
	pUI->Init();
	scene.Add(pUI);

	//Health Display -> image render + text render
	auto pLivesImageDisplay = std::make_shared<dae::GameObject>(5);
	pLivesImageDisplay->Init();
	pLivesImageDisplay->AddComponent<dae::ImageComponent>()->SetTexture("Images\\Player_Ship_Small.png");
	pLivesImageDisplay->AddComponent<dae::ImageRenderComponent>();
	pLivesImageDisplay->GetTransform()->SetLocalPosition({ 20,20,0 });
	pLivesImageDisplay->SetParent(pUI.get(), true);

	auto pLivesTextDisplay = std::make_shared<dae::GameObject>(5);
	pLivesTextDisplay->Init();
	pLivesTextDisplay->AddComponent<dae::LivesLeftComponent>()->SetPlayerIndex(0);
	pLivesTextDisplay->AddComponent<dae::TextRendererComponent>();
	auto pText = pLivesTextDisplay->GetComponent<dae::TextComponent>();

	auto fontUI = dae::ResourceManager::GetInstance().LoadFont("Galaga.ttf", 15);
	pText->SetFont(fontUI);

	pLivesTextDisplay->GetTransform()->SetLocalPosition({ 45,21,0 });
	pLivesTextDisplay->SetParent(pUI.get(), true);

	//Score Display -> text render
	auto pPointsTextDisplay = std::make_shared<dae::GameObject>(5);
	pPointsTextDisplay->Init();
	pPointsTextDisplay->AddComponent<dae::PlayerScoreComponent>()->SetPlayerIndex(0);
	pPointsTextDisplay->AddComponent<dae::TextRendererComponent>();
	pText = pPointsTextDisplay->GetComponent<dae::TextComponent>();

	pText->SetFont(fontUI);

	pPointsTextDisplay->GetTransform()->SetLocalPosition({ dae::g_WindowWidth - 150,21,0 });
	pPointsTextDisplay->SetParent(pUI.get(), true);


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

	auto pPlayer = AddPlayerGO(scene, inputManager, 0, true);
	auto pPlayerTransform = pPlayer->GetTransform().get();
	std::vector<dae::EnemyControllerComponent*> butterflyControllers{};

	AddEnemyGO(scene, pPlayerTransform, glm::vec3{ 100, 50, 0 }, dae::EnemyTypes::Butterfly, butterflyControllers);

	AddEnemyGO(scene, pPlayerTransform, glm::vec3{ 50, 50, 0 }, dae::EnemyTypes::Boss, butterflyControllers);

	LoadUI(scene);



	auto tutorial = std::make_shared<dae::GameObject>();
	tutorial->Init();
	scene.Add(tutorial);

	tutorial->AddComponent<dae::TutorialComponent>();

	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();
	LoadSounds(sounds);

	sounds->HandleSoundData(dae::SoundData{ 0, 0.1f, dae::SoundData::SoundType::Music });

}

void load()
{

	std::srand(static_cast<unsigned int>(std::time(0)));

	//Rotators();

	//Input();
	LoadGame();

}

int main(int, char* []) {


	dae::Minigin engine("../Data/", dae::g_WindowWidth, dae::g_WindowHeight);
	engine.Run(load);



	return 0;
}