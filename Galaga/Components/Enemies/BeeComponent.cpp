#include "BeeComponent.h"
#include <Components/TransformComponent.h>
#include <Engine/Timer.h>
#include <Events/EventManager.h>
#include "Events/GameEvents.h"
#include "Rendering/ResourceManager.h"
#include "Components/Collision/CollisionComponent.h"

dae::BeeComponent::BeeComponent(GameObject* owner)
	:BaseEnemyComponent(owner)
{
}


void dae::BeeComponent::Attack()
{
	m_IsAttacking = true;
	m_CurAttackState = AttackStates::Diving;
	m_MovementDir = { 0, 200, 0 };
}

void dae::BeeComponent::OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject)
{
	if (!(strcmp(hitObject.ownerType.c_str(), "PlayerAttack") == 0))
		return;


	std::unique_ptr<PointEvent> event = std::make_unique<PointEvent>();
	event->eventType = "EnemyDied";

	switch (m_CurAttackState)
	{
	case dae::BeeComponent::AttackStates::Idle:
		event->NrPoints = 50;
		break;
	case dae::BeeComponent::AttackStates::Diving:
	case dae::BeeComponent::AttackStates::Arcing:
	case dae::BeeComponent::AttackStates::Returning:
		event->NrPoints = 100;
		break;
	}

	EventManager::GetInstance().SendEventMessage(std::move(event));

	collisionOwner.owningObject->MarkForDestroy();
}

void dae::BeeComponent::Update()
{
	if (!m_IsAttacking)
		return;

	const auto elapsed = Timer::GetInstance().GetDeltaTime();

	switch (m_CurAttackState)
	{
	case dae::BeeComponent::AttackStates::Idle:
		return;
		break;
	case dae::BeeComponent::AttackStates::Diving:
		DoDiving(elapsed);
		break;
	case dae::BeeComponent::AttackStates::Arcing:
		DoArcing(elapsed);
		break;
	case dae::BeeComponent::AttackStates::Returning:
		DoReturning(elapsed);
		break;
	}
}

void dae::BeeComponent::DoDiving(const float elapsed)
{
	auto curPos = m_pTransform->GetWorldPosition();

	curPos += m_MovementDir * elapsed;

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

void dae::BeeComponent::DoArcing(const float elapsed)
{
	auto curPos = m_pTransform->GetWorldPosition();

	curPos += m_MovementDir * elapsed;

	m_pTransform->SetLocalPosition(curPos);

	m_CurArcTime += elapsed;

	if (m_CurArcTime < MAX_ARC_TIME)
		return;

	m_CurAttackState = AttackStates::Returning;

	m_MovementDir = (m_FormationPosition - curPos);

	m_MovementDir /= glm::length(m_MovementDir);
	m_MovementDir *= 200;
}

void dae::BeeComponent::DoReturning(const float elapsed)
{
	auto curPos = m_pTransform->GetWorldPosition();

	curPos += m_MovementDir * elapsed;

	m_pTransform->SetLocalPosition(curPos);

	if (glm::length(m_FormationPosition - curPos) > 1)
		return;


	m_CurAttackState = AttackStates::Idle;
	m_IsAttacking = false;
}
