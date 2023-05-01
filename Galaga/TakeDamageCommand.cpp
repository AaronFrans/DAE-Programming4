#include "TakeDamageCommand.h"
#include "Engine/GameObject.h"
#include "HealthComponent.h"

dae::TakeDamageCommand::TakeDamageCommand(GameObject* actor)
{
	m_pGameObjectHealth = actor->GetComponent<HealthComponent>().get();

}

void dae::TakeDamageCommand::Execute()
{
	m_pGameObjectHealth->TakeDamage(m_Damage);
}
