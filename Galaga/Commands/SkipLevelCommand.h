#pragma once

#include "Commands/Command.h"

namespace dae
{
	class GameOverseerComponent;
	class SkipLevelCommand : public Command
	{
	public:
		SkipLevelCommand(GameObject* actor);

		void Execute() override;


	private:

		GameOverseerComponent* m_pGOC{ nullptr };
	};


}