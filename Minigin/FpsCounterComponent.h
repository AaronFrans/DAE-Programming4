#pragma once
#include <memory>
#include "UpdatingComponent.h"
#include "TextComponent.h"
#include "Renderer.h"

namespace dae
{
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

		void CheckForRequiredComponents() const;
		void Update() override;


	private:

		void SetupRequiredComponents();
		std::weak_ptr<TextComponent> m_Text;

	};

}

