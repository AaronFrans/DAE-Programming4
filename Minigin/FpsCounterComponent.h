#pragma once
#include <memory>
#include "Component.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "Renderer.h"

namespace dae
{
	class FpsCounterComponent final :
		public Component
	{

	public:
		FpsCounterComponent();

		void CheckForRequiredComponents() const;
		void Update() override;

		void Render() const override;

	private:
		std::weak_ptr<TextComponent> m_Text;
		std::weak_ptr<TransformComponent> m_Transform;

	};

}

