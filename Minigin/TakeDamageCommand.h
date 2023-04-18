#pragma once
#include "Command.h"

namespace dae
{

	class HealthComponent;
	class TakeDamageCommand : public Command
	{
	public:
		TakeDamageCommand(GameObject* actor);

		void Execute() override;

		void SetDamage(float damage) { m_Damage = damage; };

	private:

		HealthComponent* m_pGameObjectHealth{ nullptr };

		float m_Damage{};
	};


}