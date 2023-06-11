#include "DiveCommand.h"
#include "Components/Enemies/BossComponent.h"

dae::DiveCommand::DiveCommand(GameObject* actor)
{
	m_pControlledBoss = actor->GetComponent<BossComponent>().get();
}

void dae::DiveCommand::Execute()
{
	if (!m_pControlledBoss->IsAttacking())
		m_pControlledBoss->DoDivingAttack();
}
