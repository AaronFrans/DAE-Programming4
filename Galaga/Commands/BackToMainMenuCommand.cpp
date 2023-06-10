#include "BackToMainMenuCommand.h"
#include "Components/HighScores/HighScoreComponent.h"

dae::BackToMainMenuCommand::BackToMainMenuCommand(GameObject* actor)
{
	m_pHighScores = actor->GetComponent<HighScoreComponent>().get();

}

void dae::BackToMainMenuCommand::Execute()
{
	m_pHighScores->ToMainMenu();
}
