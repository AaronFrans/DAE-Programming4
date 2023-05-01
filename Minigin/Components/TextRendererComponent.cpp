#include <stdexcept>
#include "TextRendererComponent.h"
#include "Rendering/Renderer.h"
#include "TextComponent.h"
#include "TransformComponent.h"

dae::TextRendererComponent::TextRendererComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();
}

void dae::TextRendererComponent::Render() const
{

	const auto& pos = m_Transform.lock()->GetWorldPosition();

	Renderer::GetInstance().RenderTexture(*m_Text.lock()->GetTexture(), pos.x, pos.y);

}

void dae::TextRendererComponent::CheckForRequiredComponents() const
{
	if (m_Text.expired())
	{
		throw std::invalid_argument("TextRendererComponent needs a TextComponent, currently expired");
	}
}

void dae::TextRendererComponent::SetupRequiredComponents() 
{
	auto lockedOwner = GetOwner().lock();
	if (m_Text.expired())
	{
		m_Text = lockedOwner->GetComponent<TextComponent>();

		if (m_Text.expired())
		{
			m_Text = lockedOwner->AddComponent<TextComponent>();
		}
	}

	m_Transform = lockedOwner->GetTransform();
}
