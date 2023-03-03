#include <stdexcept>
#include "TextRendererComponent.h"
#include "UpdatingComponent.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "TransformComponent.h"

dae::TextRendererComponent::TextRendererComponent(std::weak_ptr<GameObject> owner)
	:RenderComponent(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();
}

void dae::TextRendererComponent::Render() const
{
	CheckForRequiredComponents();

	const auto& pos = m_Transform.lock()->GetPosition();

	Renderer::GetInstance().RenderTexture(*m_Text.lock()->GetTexture(), pos.x, pos.y);

}

void dae::TextRendererComponent::CheckForRequiredComponents() const
{
	if (m_Text.expired())
	{
		throw std::invalid_argument("TextRendererComponent needs a TextComponent, currently expired");
	}
	if (m_Transform.expired())
	{
		throw std::invalid_argument("TextRendererComponent needs a TransformComponent, currently expired");
	}
}

void dae::TextRendererComponent::SetupRequiredComponents() 
{
	auto lockedOwner = m_Owner.lock();
	if (m_Text.expired())
	{
		m_Text = lockedOwner->GetComponent<TextComponent>();

		if (m_Text.expired())
		{
			m_Text = lockedOwner->AddComponent<TextComponent>();
		}
	}

	if (m_Transform.expired())
	{
		m_Transform = lockedOwner->GetComponent<TransformComponent>();

		if (m_Transform.expired())
		{
			m_Transform = lockedOwner->AddComponent<TransformComponent>();
		}
	}
}
