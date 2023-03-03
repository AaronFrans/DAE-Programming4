#include "ImageComponent.h"
#include "ResourceManager.h"
#include "Texture2D.h"

dae::ImageComponent::ImageComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
}

void dae::ImageComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

std::shared_ptr<dae::Texture2D> dae::ImageComponent::GetTexture() const
{
	return m_Texture;
}
