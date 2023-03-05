#pragma once
#include <memory>
#include "Component.h"
#include "Renderer.h"

namespace dae
{
	class TextComponent;
	class FpsCounterComponent final :
		public Component
	{

	public:
		FpsCounterComponent(std::weak_ptr<GameObject> owner);

		~FpsCounterComponent() = default;
		FpsCounterComponent(const FpsCounterComponent& other) = delete;
		FpsCounterComponent(FpsCounterComponent&& other) = delete;
		FpsCounterComponent& operator=(const FpsCounterComponent& other) = delete;
		FpsCounterComponent& operator=(FpsCounterComponent&& other) = delete;

		void Update() override;


	private:

		void CheckForRequiredComponents() const;
		void SetupRequiredComponents();
		std::weak_ptr<TextComponent> m_Text;
		int m_LastFps{};

	};

}

