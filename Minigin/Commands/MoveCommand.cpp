#include "MoveCommand.h"
#include "Engine/GameObject.h"
#include "Components/TransformComponent.h"
#include "Engine/Timer.h"

dae::MoveCommand::MoveCommand(GameObject* actor)
{
	m_pActor = actor;

	m_pGameObjectTransform = m_pActor->GetTransform().get();
}

void dae::MoveCommand::Execute()
{

	auto newPos = m_pGameObjectTransform->GetLocalPosition() + m_Direction * m_MoveSpeed * Timer::GetInstance().GetDeltaTime();
	m_pGameObjectTransform->SetLocalPosition(newPos);
}

