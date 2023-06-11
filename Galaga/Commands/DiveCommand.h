#pragma once
#include "Commands/Command.h"

namespace dae
{
	class BossComponent;
	class DiveCommand  final : public Command
	{
	public:
		DiveCommand(GameObject* actor);

		void Execute() override;


	private:

		BossComponent* m_pControlledBoss{ nullptr };
	};
}
