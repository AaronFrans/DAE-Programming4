#include "ButterflyComponent.h"
#include <Engine/Timer.h>
#include <Components/TransformComponent.h>
#include <Components/Collision/CollisionComponent.h>
#include <Components/Enemies/EnemyControllerComponent.h>

#include <Events/EventManager.h>
#include "Events/GameEvents.h"



void dae::ButterflyComponent::DoArcing(const float elapsed)
{
	auto curPos = m_pTransform->GetWorldPosition();

	curPos += m_MovementDir * elapsed;

	m_pTransform->SetLocalPosition(curPos);

	m_CurArcTime += elapsed;

	if (m_CurArcTime > MAX_ARC_TIME)
	{
		m_CurAttackState = AttackStates::Returning;

		m_MovementDir = (m_FormationPosition - curPos);

		m_MovementDir /= glm::length(m_MovementDir);
		m_MovementDir *= 200;
	}

}

void dae::ButterflyComponent::DoReturning(const float elapsed)
{
	auto curPos = m_pTransform->GetWorldPosition();

	curPos += m_MovementDir * elapsed;

	m_pTransform->SetLocalPosition(curPos);

	if (glm::length(m_FormationPosition - curPos) > 1)
		return;


	m_CurAttackState = AttackStates::Idle;
	m_IsAttacking = false;
}

void dae::ButterflyComponent::DoZigzagging(const float elapsed)
{
	auto curPos = m_pTransform->GetWorldPosition();
	curPos += m_MovementDir * elapsed;

	m_CurOffset += ZIGZAG_SPEED * elapsed * m_ZigzagOffsetSign;
	if (abs(m_CurOffset) > MAX_X_OFFSET)
		m_ZigzagOffsetSign *= -1;

	curPos.x = m_OriginalXPos + m_CurOffset;

	DoShooting(elapsed, curPos);

	m_pTransform->SetLocalPosition(curPos);

	if (curPos.y < m_MaxYPos)
		return;

	m_CurAttackState = AttackStates::Arcing;
	m_CurArcTime = 0;

	curPos.x > m_ScreenCenter.x ?
		m_MovementDir = { -200, 0, 0 } :
		m_MovementDir = { 200, 0, 0 };
}

void dae::ButterflyComponent::Attack()
{
	m_IsAttacking = true;
	m_CurAttackState = AttackStates::Zigzagging;
	m_MovementDir = { 0, 200, 0 };
}

void dae::ButterflyComponent::OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject)
{
	if (!(strcmp(hitObject.ownerType.c_str(), "PlayerAttack") == 0))
		return;



	std::unique_ptr<SceneEvent> pointEvent = std::make_unique<SceneEvent>();
	pointEvent->eventType = "EnemyDied";
	pointEvent->sceneName = m_SceneName;

	switch (m_CurAttackState)
	{
	case dae::ButterflyComponent::AttackStates::Idle:
		pointEvent->nrPoints = 80;
		break;
	case dae::ButterflyComponent::AttackStates::Zigzagging:
	case dae::ButterflyComponent::AttackStates::Arcing:
	case dae::ButterflyComponent::AttackStates::Returning:
		pointEvent->nrPoints = 160;
		break;
	}

	EventManager::GetInstance().SendEventMessage(std::move(pointEvent));

	std::unique_ptr<ButterflyDestroyedEvent> deathEvent = std::make_unique<ButterflyDestroyedEvent>();
	deathEvent->eventType = "ButterflyDied";
	deathEvent->sceneName = m_SceneName;

	auto pConteroller = GetOwner()->GetComponent<EnemyControllerComponent>().get();

	deathEvent->butterfly = pConteroller;

	EventManager::GetInstance().SendEventMessage(std::move(deathEvent));

	collisionOwner.owningObject->MarkForDestroy();
}

dae::ButterflyComponent::ButterflyComponent(GameObject* owner)
	:BaseEnemyComponent(owner)
{
	m_OriginalXPos = m_pTransform->GetWorldPosition().x;
}

void dae::ButterflyComponent::Update()
{

	if (!m_IsAttacking)
		return;

	const auto elapsed = Timer::GetInstance().GetDeltaTime();

	switch (m_CurAttackState)
	{
	case dae::ButterflyComponent::AttackStates::Idle:
		return;
		break;
	case dae::ButterflyComponent::AttackStates::Zigzagging:
		DoZigzagging(elapsed);
		break;
	case dae::ButterflyComponent::AttackStates::Arcing:
		DoArcing(elapsed);
		break;
	case dae::ButterflyComponent::AttackStates::Returning:
		DoReturning(elapsed);
		break;
	}
}
