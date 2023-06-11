
#include "Components/Enemies/BaseEnemyComponent.h"
#include "Components/TransformComponent.h"

#include "Rendering/ResourceManager.h"
#include "Engine/Timer.h"

#include <Components/ImageComponent.h>
#include <Components/ImageRenderComponent.h>
#include <Components/Updating/BulletMovementComponent.h>
#include <Components/Collision/CollisionComponent.h>

#include <Events/EventManager.h>
#include <Events/GameEvents.h>

void dae::BaseEnemyComponent::Update()
{
	const glm::vec3 flyInDir{0, 1, 0};

	auto curPos = m_pTransform->GetLocalPosition();

	curPos += flyInDir * m_FlySpeed * Timer::GetInstance().GetDeltaTime();

	m_pTransform->SetLocalPosition(curPos);

	if (glm::length(curPos - m_FormationPosition) > 1)
		return;

	m_HasFlownIn = true;
}

bool dae::BaseEnemyComponent::IsAttacking()
{
	return m_IsAttacking;
}

bool dae::BaseEnemyComponent::HasFlownIn()
{
	return m_HasFlownIn;
}

void dae::BaseEnemyComponent::SetFormationPosition(glm::vec3 pos)
{
	m_FormationPosition = pos;
}

void dae::BaseEnemyComponent::SetPlayerTransforms(std::vector<TransformComponent*>& player)
{
	m_pPlayerTransforms = player;
}

void dae::BaseEnemyComponent::SetScreenCenter(const glm::vec3 center)
{
	m_ScreenCenter = center;
}

void dae::BaseEnemyComponent::SetScene(const std::string& sceneName)
{
	m_SceneName = sceneName;

	m_pScene = &SceneManager::GetInstance().GetSceneByName(m_SceneName);

	std::unique_ptr<SceneEvent> event = std::make_unique<SceneEvent>();
	event->eventType = "EnemySpawned";
	event->sceneName = m_SceneName;

	EventManager::GetInstance().SendEventMessage(std::move(event));
}

void dae::BaseEnemyComponent::SetMaxYPos(const float maxYPos)
{
	m_MaxYPos = maxYPos;
}

dae::BaseEnemyComponent::BaseEnemyComponent(GameObject* owner)
	: Component(owner)
{

	m_pTransform = owner->GetTransform().get();;
	m_AttackTexture = ResourceManager::GetInstance().LoadTexture("Images\\Enemy_Attack.png");


}




void dae::BaseEnemyComponent::BulletHitCallback(const dae::CollisionData& collisionOwner, const dae::CollisionData& hitObject)
{
	if (!(strcmp(hitObject.ownerType.c_str(), "Player") == 0))
		return;

	collisionOwner.owningObject->MarkForDestroy();
}

void dae::BaseEnemyComponent::DoShooting(const float elapsed, const glm::vec3 curPos)
{
	m_CurFireCooldownTime += elapsed;

	if (curPos.y > m_MaxYPos / 2.0f)
		return;

	if (m_CurFireCooldownTime < MIN_FIRE_COOLDOWN)
		return;

	m_CurFireCooldownTime = 0;


	int randomIndex = std::rand() % m_pPlayerTransforms.size();

	auto playerPos = m_pPlayerTransforms[randomIndex]->GetLocalPosition();

	auto shootDir = playerPos - curPos;

	auto bullet = std::make_shared<GameObject>(2);
	bullet->Init();

	bullet->GetTransform()->SetLocalPosition({ curPos.x, curPos.y, curPos.z });

	bullet->AddComponent<ImageComponent>()->SetTexture(m_AttackTexture);

	auto pBulletMovement = bullet->AddComponent<BulletMovementComponent>();

	bullet->AddComponent<dae::ImageRenderComponent>();

	pBulletMovement->SetMoveDir(shootDir / glm::length(shootDir));
	pBulletMovement->SetMoveSpeed(500);

	auto collision = bullet->AddComponent<dae::CollisionComponent>();
	collision->SetCollisionData({ "EnemyAttack", bullet.get() });

	float collisionWidth{ 11 }, collisionHeight{ 22 };
	collision->SetBounds(collisionWidth, collisionHeight);
	//collision->EnableDebugSquare();


	auto boundHitCallback = std::bind(&BaseEnemyComponent::BulletHitCallback, this, std::placeholders::_1, std::placeholders::_2);
	collision->SetCallback(boundHitCallback);

	collision->SetScene(m_pScene);

	m_pScene->Add(bullet);

	m_pScene->AddCollision(collision.get());

}