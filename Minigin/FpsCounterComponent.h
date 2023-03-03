#pragma once
#include <memory>
#include "UpdatingComponent.h"
#include "Renderer.h"

namespace dae
{
	class TextComponent;
	class FpsCounterComponent final :
		public UpdatingComponent
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

