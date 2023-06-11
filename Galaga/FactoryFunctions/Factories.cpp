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
#include <Commands/DiveCommand.h>
#include <Commands/TractorCommand.h>
#include <Commands/AttackCommand.h>

#include <Events/EventManager.h>

#include "Scene/Scene.h"
#include "Input/InputManager.h"


dae::GameObject* dae::AddPlayerGO(Scene& scene, InputManager& input, unsigned playerIndex, bool useKeyboard
	, const std::string& playerTexture)
{

	auto player = std::make_shared<dae::GameObject>(1);
	player->Init();
	scene.Add(player);
	auto playerImage = player->AddComponent<dae::ImageComponent>();

	playerImage->SetTexture(playerTexture);

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
	collision->SetCollisionData({ "Player", player.get(), static_cast<int>(playerIndex) });

	float collisionWidth{ imageWidth }, collisionHeight{ imageHeight };
	collision->SetBounds(collisionWidth, collisionHeight);

	collision->SetCallback([&](const dae::CollisionData& ownerObject, const dae::CollisionData& hitObject) {

		if ((strcmp(hitObject.ownerType.c_str(), "EnemyAttack") != 0) && (strcmp(hitObject.ownerType.c_str(), "Enemy") != 0))
			return;

		std::unique_ptr<dae::PlayerEvent> event = std::make_unique<dae::PlayerEvent>();
		event->eventType = "PlayerDied";
		event->playerIndex = ownerObject.playerIndex;
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


dae::GameObject* dae::AddEnemyGO(dae::Scene& scene, std::vector<TransformComponent*>& pPlayer, glm::vec3 formationPos, dae::EnemyTypes enemyType,
	std::vector<EnemyControllerComponent*>& butterflies)
{


	auto enemy = std::make_shared<dae::GameObject>(0);
	enemy->Init();
	scene.Add(enemy);

	enemy->GetTransform()->SetLocalPosition(formationPos - glm::vec3{0, 200, 0});

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
	attacking->SetFormationPosition(formationPos);
	attacking->SetPlayerTransforms(pPlayer);
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

dae::GameObject* dae::AddPlayerEnemyGO(dae::Scene& scene, std::vector<TransformComponent*>& pPlayer, glm::vec3 formationPos, std::vector<EnemyControllerComponent*>& butterflies)
{

	auto sceneName = scene.GetName();

	auto enemy = std::make_shared<dae::GameObject>(0);
	enemy->Init();
	scene.Add(enemy);

	enemy->GetTransform()->SetLocalPosition(formationPos - glm::vec3{0, 200, 0});


	AddEnemyTexture(enemy, "Images\\Player_Galaga_Boss.png");
	auto boss = enemy->AddComponent<dae::BossComponent>();
	boss->SetButterflies(butterflies);

	boss->SetFormationPosition(formationPos);
	boss->SetPlayerTransforms(pPlayer);
	boss->SetScreenCenter({ dae::g_WindowWidth / 2.0f, dae::g_WindowHeight / 2.0f, 0 });
	boss->SetScene(scene.GetName());
	boss->SetMaxYPos(dae::g_WindowHeight - 20);
	boss->SetDamagedTexture(ResourceManager::GetInstance().LoadTexture("Images\\Player_Galaga_Boss_Damaged.png"));
	boss->SetupCollision();


	auto playerImage = enemy->GetComponent<dae::ImageComponent>();

	const float imageWidth = playerImage->GetTextureWidth(),
		imageHeight = playerImage->GetTextureHeight();

	auto collision = enemy->AddComponent<dae::CollisionComponent>();
	scene.AddCollision(collision.get());
	collision->SetCollisionData({ "Enemy", enemy.get() });

	float collisionWidth{ imageWidth }, collisionHeight{ imageHeight };
	collision->SetBounds(collisionWidth, collisionHeight);
	//collision->EnableDebugSquare();

	auto boundHitCallback = std::bind(&dae::BaseEnemyComponent::OnHitCallback, boss, std::placeholders::_1, std::placeholders::_2);
	collision->SetCallback(boundHitCallback);

	collision->SetScene(&scene);


	auto& inputManager = dae::InputManager::GetInstance();


	inputManager.AddXboxCommand<dae::TractorCommand>(std::make_unique<dae::TractorCommand>(enemy.get()),
		dae::XboxControllerInput{ 0, dae::XboxController::ControllerButton::ButtonA, dae::ButtonState::Up, sceneName });

	inputManager.AddXboxCommand<dae::DiveCommand>(std::make_unique<dae::DiveCommand>(enemy.get()),
		dae::XboxControllerInput{ 0, dae::XboxController::ControllerButton::ButtonB, dae::ButtonState::Up, sceneName });

	return enemy.get();

}

void dae::LoadUI(dae::Scene& scene, const std::string& nextScene, bool useSecondPlayer)
{

	auto pUI = std::make_shared<dae::GameObject>();
	pUI->Init();
	scene.Add(pUI);

	auto sceneName = scene.GetName();
	auto fontUI = dae::ResourceManager::GetInstance().LoadFont("Galaga.ttf", 15);

	//Score Display -> text render
	auto pPointsTextDisplay = std::make_shared<dae::GameObject>(5);
	pPointsTextDisplay->Init();
	auto pScoreComp = pPointsTextDisplay->AddComponent<dae::PlayerScoreComponent>();
	pScoreComp->SetPlayerIndex(0);
	pScoreComp->SetSceneName(sceneName);
	pScoreComp->SetNextSceneName(nextScene);
	pPointsTextDisplay->AddComponent<dae::TextRendererComponent>();
	auto pText = pPointsTextDisplay->GetComponent<dae::TextComponent>();

	pText->SetFont(fontUI);

	pPointsTextDisplay->GetTransform()->SetLocalPosition({ dae::g_WindowWidth - 230,21,0 });
	pPointsTextDisplay->SetParent(pUI.get(), true);


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
	pText = pLivesTextDisplay->GetComponent<dae::TextComponent>();

	pText->SetFont(fontUI);

	pLivesTextDisplay->GetTransform()->SetLocalPosition({ 45,21,0 });
	pLivesTextDisplay->SetParent(pUI.get(), true);

	if (!useSecondPlayer)
		return;

	auto pSecondLivesImageDisplay = std::make_shared<dae::GameObject>(5);
	pSecondLivesImageDisplay->Init();
	pSecondLivesImageDisplay->AddComponent<dae::ImageComponent>()->SetTexture("Images\\Player_2_Ship_Small.png");
	pSecondLivesImageDisplay->AddComponent<dae::ImageRenderComponent>();
	pSecondLivesImageDisplay->GetTransform()->SetLocalPosition({ 20,40,0 });
	pSecondLivesImageDisplay->SetParent(pUI.get(), true);

	auto pSecondLivesTextDisplay = std::make_shared<dae::GameObject>(5);
	pSecondLivesTextDisplay->Init();
	pLivesTextDisplay->AddComponent<dae::LivesLeftComponent>()->SetPlayerIndex(1);
	pSecondLivesTextDisplay->AddComponent<dae::LivesLeftComponent>()->SetPlayerIndex(0);
	pSecondLivesTextDisplay->AddComponent<dae::TextRendererComponent>();
	pText = pSecondLivesTextDisplay->GetComponent<dae::TextComponent>();

	pText->SetFont(fontUI);

	pSecondLivesTextDisplay->GetTransform()->SetLocalPosition({ 45,41,0 });
	pSecondLivesTextDisplay->SetParent(pUI.get(), true);


}
