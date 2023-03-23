#include "MoveCommand.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"

dae::MoveCommand::MoveCommand(GameObject* actor)
	:Command(actor)
{
	m_pGameObjectTransform = GetActor()->GetTransform().get();
}

void dae::MoveCommand::Execute()
{
	GetActor()->GetTransform();

	auto newPos = m_pGameObjectTransform->GetLocalPosition() + m_Direction * m_MoveSpeed * Timer::GetInstance().GetTimeStep();
	m_pGameObjectTransform->SetLocalPosition(newPos);
}

