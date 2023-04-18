#pragma once
#include "Component.h"
#include "imgui.h"
#include "string"

namespace dae
{
	class TutorialComponent final :
		public Component
	{
	public:
		TutorialComponent(std::weak_ptr<GameObject> owner);

		~TutorialComponent() = default;
		TutorialComponent(const TutorialComponent& other) = delete;
		TutorialComponent(TutorialComponent&& other) = delete;
		TutorialComponent& operator=(const TutorialComponent& other) = delete;
		TutorialComponent& operator=(TutorialComponent&& other) = delete;

		void RenderImGui() override;

	private:


	};
}

