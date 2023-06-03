#include <stdexcept>
#include "ImageRenderComponent.h"
#include "Rendering/Renderer.h"
#include "ImageComponent.h"
#include "TransformComponent.h"

#include "Events/EventManager.h"

dae::ImageRenderComponent::ImageRenderComponent(GameObject* owner)
	:Component(owner)
{

	SetupRequiredComponents();
	CheckForRequiredComponents();
}

void dae::ImageRenderComponent::Render() const
{
	const auto& pos = m_Transform->GetWorldPosition();

	Renderer::GetInstance().RenderTexture(*m_Image->GetTexture(), pos.x, pos.y);
}

void dae::ImageRenderComponent::CheckForRequiredComponents() const
{
	if (!m_Image)
	{
		throw std::invalid_argument("ImageRenderComponent needs a ImageComponent, currently expired");
	}
	if (!m_Transform)
	{
		throw std::invalid_argument("ImageRenderComponent needs a TransformComponent, currently expired");
	}
}

void dae::ImageRenderComponent::SetupRequiredComponents()
{
	auto lockedOwner = GetOwner();
	if (!m_Image)
	{
		m_Image = lockedOwner->GetComponent<ImageComponent>().get();

		if (!m_Image)
		{
			m_Image = lockedOwner->AddComponent<ImageComponent>().get();
		}
	}

	if (!m_Transform)
	{
		m_Transform = lockedOwner->GetComponent<TransformComponent>().get();

		if (!m_Transform)
		{
			m_Transform = lockedOwner->AddComponent<TransformComponent>().get();
		}
	}

}
