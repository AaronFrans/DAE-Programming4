#pragma once
#include "Commands/Command.h"

namespace dae
{
	class MainMenuComponent;
	class ConfirmCommand  final : public Command
	{
	public:
		ConfirmCommand(GameObject* actor);

		void Execute() override;


	private:

		MainMenuComponent* m_pMenu{ nullptr };
	};


}