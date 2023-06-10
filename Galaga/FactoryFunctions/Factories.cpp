#include "Factories.h"
#include <SDL.h>

#include <Engine/GameObject.h>

#include <Components/ImageComponent.h>
#include <Components/TextComponent.h>
#include <Components/TransformComponent.h>
#include <Components/ImageRenderComponent.h>
#include <Components/Updating/AttackComponent.h>
#include <Components/Observers/LivesLeftComponent.h>
#include <Components/TextRendererComponent.h>
#include <Components/Enemies/EnemyControllerComponent.h>
#include <Components/Enemies/BeeComponent.h>
#include <Components/Enemies/ButterflyComponent.h>
#include <Components/Enemies/BossComponent.h>
#include <Components/Observers/PlayerScoreComponent.h>

#include <Rendering/ResourceManager.h>

#include <Commands/RestrictedMovementCommand.h>
#include <Commands/AttackCommand.h>

#include <Events/EventManager.h>

#include "Scene/Scene.h"
#include "Input/InputManager.h"


dae::GameObject* dae::AddPlayerGO(Scene& scene, InputManager& input, unsigned playerIndex, bool useKeyboard)
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

	auto sceneName = scene.GetName();

	if (useKeyboard)
	{
		auto pMoveCommand = input.AddKeyboardCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed, sceneName });

		pMoveCommand->SetDirection(playerMoveDirection);
		pMoveCommand->SetPlayerIndex(playerIndex);

		pMoveCommand->SetMinPoint(playerMinPoint);
		pMoveCommand->SetMaxPoint(playerMaxPoint);

		pMoveCommand->SetMoveSpeed(-playerMoveSpeed);

		pMoveCommand = input.AddKeyboardCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed, sceneName });

		pMoveCommand->SetDirection(playerMoveDirection);
		pMoveCommand->SetPlayerIndex(playerIndex);

		pMoveCommand->SetMinPoint(playerMinPoint);
		pMoveCommand->SetMaxPoint(playerMaxPoint);

		pMoveCommand->SetMoveSpeed(playerMoveSpeed);

		input.AddKeyboardCommand<dae::AttackCommand>(std::make_unique<dae::AttackCommand>(player.get()),
			dae::KeyboardInput{ SDL_SCANCODE_F, dae::ButtonState::Up, sceneName });
	}

	auto pMoveCommand = input.AddXboxCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
		dae::XboxControllerInput{ playerIndex, dae::XboxController::ControllerButton::DPadLeft, dae::ButtonState::Pressed, sceneName });

	pMoveCommand->SetDirection(playerMoveDirection);
	pMoveCommand->SetPlayerIndex(playerIndex);

	pMoveCommand->SetMinPoint(playerMinPoint);
	pMoveCommand->SetMaxPoint(playerMaxPoint);

	pMoveCommand->SetMoveSpeed(-playerMoveSpeed);

	pMoveCommand = input.AddXboxCommand<dae::RestrictedMovementCommand>(std::make_unique<dae::RestrictedMovementCommand>(player.get()),
		dae::XboxControllerInput{ playerIndex, dae::XboxController::ControllerButton::DPadRight, dae::ButtonState::Pressed, sceneName });

	pMoveCommand->SetDirection(playerMoveDirection);
	pMoveCommand->SetPlayerIndex(playerIndex);

	pMoveCommand->SetMinPoint(playerMinPoint);
	pMoveCommand->SetMaxPoint(playerMaxPoint);

	pMoveCommand->SetMoveSpeed(playerMoveSpeed);

	input.AddXboxCommand<dae::AttackCommand>(std::make_unique<dae::AttackCommand>(player.get()),
		dae::XboxControllerInput{ playerIndex, dae::XboxController::ControllerButton::ButtonX, dae::ButtonState::Up, sceneName });


	auto collision = player->AddComponent<dae::CollisionComponent>();
	scene.AddCollision(collision.get());
	collision->SetCollisionData({ "Player", player.get() });

	float collisionWidth{ imageWidth }, collisionHeight{ imageHeight };
	collision->SetBounds(collisionWidth, collisionHeight);

	collision->SetCallback([&](const dae::CollisionData&, const dae::CollisionData& hitObject) {

		if ((strcmp(hitObject.ownerType.c_str(), "EnemyAttack") != 0) && (strcmp(hitObject.ownerType.c_str(), "Enemy") != 0))
			return;

		std::unique_ptr<dae::PlayerEvent> event = std::make_unique<dae::PlayerEvent>();
		event->eventType = "PlayerDied";
		event->playerIndex = 0;
		dae::EventManager::GetInstance().SendEventMessage(std::move(event));
		});

	collision->SetScene(&scene);

	return player.get();
}

void dae::AddEnemyTexture(std::shared_ptr<GameObject> enemy, const std::string& texturePath)
{

	auto playerImage = enemy->AddComponent<dae::ImageComponent>();

	playerImage->SetTexture(texturePath);
	enemy->AddComponent<dae::ImageRenderComponent>();
}


dae::GameObject* dae::AddEnemyGO(dae::Scene& scene, TransformComponent* pPlayer, glm::vec3 pos, dae::EnemyTypes enemyType,
	std::vector<EnemyControllerComponent*>& butterflies)
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

void dae::LoadUI(dae::Scene& scene, const std::string& nextScene)
{

	auto pUI = std::make_shared<dae::GameObject>();
	pUI->Init();
	scene.Add(pUI);

	auto sceneName = scene.GetName();

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
	auto pScoreComp = pPointsTextDisplay->AddComponent<dae::PlayerScoreComponent>();
	pScoreComp->SetPlayerIndex(0);
	pScoreComp->SetSceneName(sceneName);
	pScoreComp->SetNextSceneName(nextScene);
	pPointsTextDisplay->AddComponent<dae::TextRendererComponent>();
	pText = pPointsTextDisplay->GetComponent<dae::TextComponent>();

	pText->SetFont(fontUI);

	pPointsTextDisplay->GetTransform()->SetLocalPosition({ dae::g_WindowWidth - 150,21,0 });
	pPointsTextDisplay->SetParent(pUI.get(), true);

}
