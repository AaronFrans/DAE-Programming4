#include "SwitchButtonCommand.h"
#include "Components/Rendering/MainMenuComponent.h"

dae::SwitchButtonCommand::SwitchButtonCommand(GameObject* actor)
{
	m_pMenu = actor->GetComponent<MainMenuComponent>().get();
}

void dae::SwitchButtonCommand::Execute()
{
	m_pMenu->SwitchButton(m_ChooseNext);
}
