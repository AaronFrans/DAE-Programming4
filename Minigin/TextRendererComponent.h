#pragma once
#include "RenderComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"

namespace dae
{
	class TextRendererComponent final :
		public RenderComponent
	{

	public:
		TextRendererComponent();
		TextRendererComponent(std::weak_ptr<GameObject> owner);

		~TextRendererComponent() = default;
		TextRendererComponent(const TextRendererComponent& other) = delete;
		TextRendererComponent(TextRendererComponent&& other) = delete;
		TextRendererComponent& operator=(const TextRendererComponent& other) = delete;
		TextRendererComponent& operator=(TextRendererComponent&& other) = delete;

		void Render() const override;
		void CheckForRequiredComponents() const;

	private:
		void SetupRequiredComponents();

		std::weak_ptr<TextComponent> m_Text;
		std::weak_ptr<TransformComponent> m_Transform;

	};
}

