#include "RestrictedMovementCommand.h"
#include "Engine/GameObject.h"
#include "Engine/Timer.h"
#include "Components/TransformComponent.h"

dae::RestrictedMovementCommand::RestrictedMovementCommand(GameObject* actor)
{
	m_pGameObjectTransform = actor->GetTransform().get();

}

void dae::RestrictedMovementCommand::Execute()
{
	auto newPos = m_pGameObjectTransform->GetLocalPosition() + m_Direction * m_MoveSpeed * Timer::GetInstance().GetDeltaTime();

	newPos = glm::clamp(newPos, m_MinPoint, m_MaxPoint);

	m_pGameObjectTransform->SetLocalPosition(newPos);
}
