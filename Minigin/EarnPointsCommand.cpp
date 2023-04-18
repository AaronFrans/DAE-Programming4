#include "EarnPointsCommand.h"
#include "EarnPointsComponent.h"

dae::EarnPointsCommand::EarnPointsCommand(GameObject* actor)
{
	m_pEarnPoints = actor->GetComponent<EarnPointsComponent>().get();
}

void dae::EarnPointsCommand::Execute()
{
	m_pEarnPoints->EarnPoints();
}
