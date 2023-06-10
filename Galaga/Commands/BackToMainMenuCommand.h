#pragma once
#include "Commands/Command.h"

namespace dae
{
	class HighScoreComponent;
	class BackToMainMenuCommand : public Command
	{
	public:
		BackToMainMenuCommand(GameObject* actor);

		void Execute() override;


	private:

		HighScoreComponent* m_pHighScores{ nullptr };
	};


}