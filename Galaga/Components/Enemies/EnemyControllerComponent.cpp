#include "EnemyControllerComponent.h"
#include "BaseEnemyComponent.h"
#include <Engine/Timer.h>



dae::EnemyControllerComponent::EnemyControllerComponent(GameObject* owner)
	: Component(owner)
{
	m_pControlledEnemy = owner->GetComponent<BaseEnemyComponent>().get();
	assert(m_pControlledEnemy && "An enemyController must have a BaseEnemyComponent to controll");
}

void dae::EnemyControllerComponent::Update()
{
	if (m_pControlledEnemy->IsAttacking())
		return;

	m_CurAttackDelay += Timer::GetInstance().GetDeltaTime();

	if (m_CurAttackDelay < m_MinAttackDelay)
		return;

	m_pControlledEnemy->Attack();
	m_CurAttackDelay = 0;
}
