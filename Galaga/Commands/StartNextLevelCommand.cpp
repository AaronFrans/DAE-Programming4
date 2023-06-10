#include "StartNextLevelCommand.h"
#include "Components/Rendering/DetailsComponent.h"

dae::StartNextLevelCommand::StartNextLevelCommand(GameObject* actor)
{
	m_pDetails = actor->GetComponent<DetailsComponent>().get();
}

void dae::StartNextLevelCommand::Execute()
{
	m_pDetails->GoToNextScene();
}
