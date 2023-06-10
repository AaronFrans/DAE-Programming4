#pragma once
#include "Commands/Command.h"

namespace dae
{
	class MainMenuComponent;
	class SwitchButtonCommand : public Command
	{
	public:
		SwitchButtonCommand(GameObject* actor);

		void Execute() override;

		void SetMoveNext(bool chooseNext) { m_ChooseNext = chooseNext; };
	private:

		MainMenuComponent* m_pMenu{ nullptr };

		bool m_ChooseNext{ false };
	};


}