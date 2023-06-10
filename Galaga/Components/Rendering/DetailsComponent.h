#pragma once
#include "Components/Component.h"

namespace dae
{
	class TextRendererComponent;
	class TextComponent;
	class DetailsComponent final : public Component
	{
	public:
		DetailsComponent(GameObject* owner);


		~DetailsComponent() = default;
		DetailsComponent(const DetailsComponent& other) = delete;
		DetailsComponent(DetailsComponent&& other) = delete;
		DetailsComponent& operator=(const DetailsComponent& other) = delete;
		DetailsComponent& operator=(DetailsComponent&& other) = delete;

		void SetActive(bool isActive);

		void GoToNextScene();

		void SetDetailsText(const int nrShots, const int NrHits);
		void SetNextSceneName(const std::string& name) { m_NextSceneName = name; };

	private:
		TextRendererComponent* m_ShotsRender{};
		TextRendererComponent* m_HitRender{};
		TextRendererComponent* m_PercentRender{};

		TextComponent* m_ShotsText{};
		TextComponent* m_HitText{};
		TextComponent* m_PercentText{};

		std::string m_NextSceneName{};

	};
}

