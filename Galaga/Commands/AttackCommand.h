#pragma once
#include "Commands/Command.h"

namespace dae
{
	class AttackComponent;
	class AttackCommand final : public Command
	{
	public:
		AttackCommand(GameObject* actor);

		void Execute() override;

	private:
		AttackComponent* m_pGameObjectAttack{ nullptr };
	};
}

