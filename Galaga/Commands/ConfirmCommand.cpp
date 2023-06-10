#include "ConfirmCommand.h"
#include "Components/Rendering/MainMenuComponent.h"

dae::ConfirmCommand::ConfirmCommand(GameObject* actor)
{
	m_pMenu = actor->GetComponent<MainMenuComponent>().get();
}

void dae::ConfirmCommand::Execute()
{
	m_pMenu->SelectButton();
}
