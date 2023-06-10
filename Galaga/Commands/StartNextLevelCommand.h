#pragma once
#include "Commands/Command.h"

namespace dae
{
	class DetailsComponent;
	class StartNextLevelCommand : public Command
	{
	public:
		StartNextLevelCommand(GameObject* actor);

		void Execute() override;


	private:

		DetailsComponent* m_pDetails{ nullptr };
	};


}
