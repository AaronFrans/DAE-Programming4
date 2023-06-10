#pragma once
#include "Components/Component.h"

namespace dae
{
	struct Event;
	class HighScoreComponent final : public Component
	{
	public:

		HighScoreComponent(GameObject* owner);


		~HighScoreComponent();
		HighScoreComponent(const HighScoreComponent& other) = delete;
		HighScoreComponent(HighScoreComponent&& other) = delete;
		HighScoreComponent& operator=(const HighScoreComponent& other) = delete;
		HighScoreComponent& operator=(HighScoreComponent&& other) = delete;

		void ReplaceChildren();

		void ToMainMenu();

	private:

		void FinalScoreSent(const Event* e);

		std::vector<int> m_Scores{};
	};

}
