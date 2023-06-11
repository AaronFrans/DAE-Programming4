#include "TractorCommand.h"
#include "Components/Enemies/BossComponent.h"

dae::TractorCommand::TractorCommand(GameObject* actor)
{
	m_pControlledBoss = actor->GetComponent<BossComponent>().get();
}

void dae::TractorCommand::Execute()
{
	if (!m_pControlledBoss->IsAttacking())
		m_pControlledBoss->DoTractorBeamAttack();
}
