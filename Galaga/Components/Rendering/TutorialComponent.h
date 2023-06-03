#pragma once
#include "Components/Component.h"
#include "imgui.h"
#include "string"

namespace dae
{
	class TutorialComponent final :
		public Component
	{
	public:
		TutorialComponent(GameObject* owner);

		~TutorialComponent() = default;
		TutorialComponent(const TutorialComponent& other) = delete;
		TutorialComponent(TutorialComponent&& other) = delete;
		TutorialComponent& operator=(const TutorialComponent& other) = delete;
		TutorialComponent& operator=(TutorialComponent&& other) = delete;

		void RenderImGui() override;

	private:


	};
}

