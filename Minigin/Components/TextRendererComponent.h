#pragma once
#include "Component.h"

namespace dae
{
	class TextComponent;
	class TransformComponent;

	class TextRendererComponent final :
		public Component
	{

	public:
		TextRendererComponent(GameObject* owner);

		~TextRendererComponent() = default;
		TextRendererComponent(const TextRendererComponent& other) = delete;
		TextRendererComponent(TextRendererComponent&& other) = delete;
		TextRendererComponent& operator=(const TextRendererComponent& other) = delete;
		TextRendererComponent& operator=(TextRendererComponent&& other) = delete;

		void Render() const override;

	private:
		void CheckForRequiredComponents() const;
		void SetupRequiredComponents();

		TextComponent* m_Text{};
		TransformComponent* m_Transform{};

	};
}

