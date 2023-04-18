#pragma once
#include "Component.h"

namespace dae
{

	class EarnPointsComponent final : public Component
	{
	public:
		EarnPointsComponent(std::weak_ptr<GameObject> owner);

		~EarnPointsComponent() = default;
		EarnPointsComponent(const EarnPointsComponent& other) = delete;
		EarnPointsComponent(EarnPointsComponent&& other) = delete;
		EarnPointsComponent& operator=(const EarnPointsComponent& other) = delete;
		EarnPointsComponent& operator=(EarnPointsComponent&& other) = delete;

		void SetPlayerIndex(unsigned playerIndex);

		void EarnPoints();


	private:

		unsigned m_PlayerIndex{ 0 };
	};

};

