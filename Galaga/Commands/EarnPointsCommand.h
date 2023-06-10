#pragma once
#include "Commands/Command.h"


namespace dae
{
	class EarnPointsComponent;
	class EarnPointsCommand final : public Command
	{
	public:
		EarnPointsCommand(GameObject* actor);

		void Execute() override;


	private:

		EarnPointsComponent* m_pEarnPoints{ nullptr };

		float m_Damage{};
	};


}

