#pragma once
#include "Commands/Command.h"

namespace dae
{
	class BossComponent;
	class TractorCommand  final : public Command
	{
	public:
		TractorCommand(GameObject* actor);

		void Execute() override;


	private:

		BossComponent* m_pControlledBoss{ nullptr };
	};
}

