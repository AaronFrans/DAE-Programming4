#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include "Texture2D.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final : public Component
	{
	public:
		TextComponent() = default;

		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<Font> font);
		std::shared_ptr<Texture2D> GetTexture() const;



	private:
		void SetTexture();
		std::string m_Text{ "default text" };
		std::shared_ptr<Font> m_Font;

		std::shared_ptr<Texture2D> m_Texture;
	};
}
