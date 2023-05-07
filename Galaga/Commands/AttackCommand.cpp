#include "AttackCommand.h"
#include "Components/Updating/AttackComponent.h"

dae::AttackCommand::AttackCommand(GameObject* actor)
{
	m_pGameObjectAttack = actor->GetComponent<AttackComponent>().get();
}

void dae::AttackCommand::Execute()
{
	m_pGameObjectAttack->Attack();

}
