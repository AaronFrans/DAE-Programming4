#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent()
	: Component(),
	m_text("text"),
	m_font(),
	m_textTexture(nullptr)
{ }

void dae::TextComponent::Update()
{

}


// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
}