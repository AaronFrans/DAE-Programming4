#include <stdexcept>
#include "TextRendererComponent.h"
#include "Rendering/Renderer.h"
#include "TextComponent.h"
#include "TransformComponent.h"

dae::TextRendererComponent::TextRendererComponent(GameObject* owner)
	:Component(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();
}

void dae::TextRendererComponent::Render() const
{

	const auto& pos = m_Transform->GetWorldPosition();

	Renderer::GetInstance().RenderTexture(*m_Text->GetTexture(), pos.x, pos.y);

}

void dae::TextRendererComponent::CheckForRequiredComponents() const
{
	if (!m_Text)
	{
		throw std::invalid_argument("TextRendererComponent needs a TextComponent, currently expired");
	}
}

void dae::TextRendererComponent::SetupRequiredComponents() 
{
	auto Owner = GetOwner();
	if (!m_Text)
	{
		m_Text = Owner->GetComponent<TextComponent>().get();

		if (!m_Text)
		{
			m_Text = Owner->AddComponent<TextComponent>().get();
		}
	}

	m_Transform = Owner->GetTransform().get();
}
