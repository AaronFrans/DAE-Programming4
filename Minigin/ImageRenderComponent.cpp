#include <stdexcept>
#include "ImageRenderComponent.h"
#include "Renderer.h"
#include "ImageComponent.h"
#include "TransformComponent.h"


dae::ImageRenderComponent::ImageRenderComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
	SetupRequiredComponents();
	CheckForRequiredComponents();
}

void dae::ImageRenderComponent::Render() const
{
	const auto& pos = m_Transform.lock()->GetWorldPosition();

	Renderer::GetInstance().RenderTexture(*m_Image.lock()->GetTexture(), pos.x, pos.y);
}

void dae::ImageRenderComponent::CheckForRequiredComponents() const
{
	if (m_Image.expired())
	{
		throw std::invalid_argument("ImageRenderComponent needs a ImageComponent, currently expired");
	}
	if (m_Transform.expired())
	{
		throw std::invalid_argument("ImageRenderComponent needs a TransformComponent, currently expired");
	}
}

void dae::ImageRenderComponent::SetupRequiredComponents()
{
	auto lockedOwner = GetOwner().lock();
	if (m_Image.expired())
	{
		m_Image = lockedOwner->GetComponent<ImageComponent>();

		if (m_Image.expired())
		{
			m_Image = lockedOwner->AddComponent<ImageComponent>();
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
