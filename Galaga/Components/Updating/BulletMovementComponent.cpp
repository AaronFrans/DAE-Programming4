#include "BulletMovementComponent.h"
#include "Components/TransformComponent.h"
#include "Engine/Timer.h"

dae::BulletMovementComponent::BulletMovementComponent(GameObject* owner)
	:Component{ owner }
{
	m_pBulletTransform = owner->GetTransform().get();
}

void dae::BulletMovementComponent::Update()
{
	auto currentPos = m_pBulletTransform->GetWorldPosition();
	const float distanceThisUpdate{ m_MoveSpeed * Timer::GetInstance().GetDeltaTime() };
	currentPos.y -= distanceThisUpdate;
	m_DistanceTraveled += distanceThisUpdate;
	m_pBulletTransform->SetLocalPosition(currentPos);

	if (m_DistanceTraveled >= m_MaxDistance)
	{
		GetOwner()->MarkForDestroy();
	}
}

void dae::BulletMovementComponent::SetMoveSpeed(float moveSpeed)
{
	m_MoveSpeed = moveSpeed;
}

void dae::BulletMovementComponent::SetMaxDistance(float maxDistance)
{
	m_MaxDistance = maxDistance;
}
