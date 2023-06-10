#include "SkipLevelCommand.h"
#include "Components/Game/GameOverseerComponent.h"

dae::SkipLevelCommand::SkipLevelCommand(GameObject* actor)
{
	m_pGOC = actor->AddComponent<GameOverseerComponent>().get();
}

void dae::SkipLevelCommand::Execute()
{
	m_pGOC->SkipLevel();
}
