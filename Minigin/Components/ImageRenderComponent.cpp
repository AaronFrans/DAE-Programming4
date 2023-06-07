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

	if (!m_IsActive)
		return;

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
	auto owner = GetOwner();
	if (!m_Image)
	{
		m_Image = owner->GetComponent<ImageComponent>().get();

		if (!m_Image)
		{
			m_Image = owner->AddComponent<ImageComponent>().get();
		}
	}

	m_Transform = owner->GetTransform().get();

}
