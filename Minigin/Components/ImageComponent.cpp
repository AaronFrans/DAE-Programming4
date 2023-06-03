#include "ImageComponent.h"
#include "Rendering/ResourceManager.h"
#include "Rendering/Texture2D.h"

dae::ImageComponent::ImageComponent(GameObject* owner)
	:Component(owner)
{
}

void dae::ImageComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::ImageComponent::SetTexture(std::shared_ptr<Texture2D>& texture)
{
	m_Texture = texture;
}

glm::vec2 dae::ImageComponent::GetTextureDimensions() const
{
	return static_cast<glm::vec2>(m_Texture->GetSize());
}

std::shared_ptr<dae::Texture2D> dae::ImageComponent::GetTexture() const
{
	return m_Texture;
}

float dae::ImageComponent::GetTextureWidth() const
{
	return static_cast<float>(m_Texture->GetSize().x);
}

float dae::ImageComponent::GetTextureHeight() const
{
	return static_cast<float>(m_Texture->GetSize().y);
}
