#pragma once
#include <memory>
#include "Component.h"
#include "Rendering/Renderer.h"

namespace dae
{
	class TextComponent;
	class FpsCounterComponent final :
		public Component
	{

	public:
		FpsCounterComponent(GameObject* owner);

		~FpsCounterComponent() = default;
		FpsCounterComponent(const FpsCounterComponent& other) = delete;
		FpsCounterComponent(FpsCounterComponent&& other) = delete;
		FpsCounterComponent& operator=(const FpsCounterComponent& other) = delete;
		FpsCounterComponent& operator=(FpsCounterComponent&& other) = delete;

		void Update() override;


	private:

		void CheckForRequiredComponents() const;
		void SetupRequiredComponents();
		TextComponent* m_Text{};
		int m_LastFps{};

	};

}

