#include <stdexcept>
#include <SDL_ttf.h>
#include <cassert>
#include "TextComponent.h"
#include "Font.h"
#include "Renderer.h"
#include "Texture2D.h"




dae::TextComponent::TextComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
	
}

std::shared_ptr<dae::Texture2D> dae::TextComponent::GetTexture()
{
	if (m_NeedsUpdate)
	{
		SetTexture();
	}
	return m_Texture;
}

void dae::TextComponent::SetTexture()
{
	const SDL_Color color = { 255,255,255 }; // only white text is supported now

	assert(m_Font && "No Font set for TextComponent");

	const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);

	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}


	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_Texture = std::make_shared<Texture2D>(texture);

}


void dae::TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_Font = std::move(font);
	m_NeedsUpdate = true;
}