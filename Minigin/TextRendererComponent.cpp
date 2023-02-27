#include "TextRendererComponent.h"
#include "Renderer.h"
#include "UpdatingComponent.h"

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
	assert(m_Owner.lock()->GetComponent<TextComponent>()
		&& "TextRendererComponent needs a text component");
	assert(m_Owner.lock()->GetComponent<TransformComponent>()
		&& "TextRendererComponent needs a text component");
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
