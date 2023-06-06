#include "Components/Enemies/BaseEnemyComponent.h"

bool dae::BaseEnemyComponent::IsAttacking()
{
	return m_IsAttacking;
}

void dae::BaseEnemyComponent::SetFormationPosition(glm::vec3 pos)
{
	m_FormationPosition = pos;
}

dae::BaseEnemyComponent::BaseEnemyComponent(GameObject* owner)
	: Component(owner)
{
}
