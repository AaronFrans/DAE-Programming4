#pragma once
#include <string>
#include <memory>
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final : public Component
	{
	public:
		TextComponent();

		void SetText(const std::string& text);

		virtual	void Update() override;

	private:
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
