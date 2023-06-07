
#include "Components/Enemies/BaseEnemyComponent.h"
#include "Components/TransformComponent.h"
#include "Rendering/ResourceManager.h"
#include <Components/ImageComponent.h>
#include <Components/ImageRenderComponent.h>
#include <Components/Updating/BulletMovementComponent.h>
#include <Components/Collision/CollisionComponent.h>

bool dae::BaseEnemyComponent::IsAttacking()
{
	return m_IsAttacking;
}

void dae::BaseEnemyComponent::SetFormationPosition(glm::vec3 pos)
{
	m_FormationPosition = pos;
}

void dae::BaseEnemyComponent::SetPlayerTransform(const TransformComponent* player)
{
	m_pPlayerTransform = player;
}

void dae::BaseEnemyComponent::SetScreenCenter(const glm::vec3 center)
{
	m_ScreenCenter = center;
}

void dae::BaseEnemyComponent::SetScene(const std::string& sceneName)
{
	m_SceneName = sceneName;

	m_pScene = &SceneManager::GetInstance().GetSceneByName(m_SceneName);
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

	if (m_CurFireCooldownTime < MIN_FIRE_COOLDOWN)
		return;

	m_CurFireCooldownTime = 0;

	auto playerPos = m_pPlayerTransform->GetLocalPosition();

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