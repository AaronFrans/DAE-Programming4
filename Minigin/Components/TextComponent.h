#pragma once
#include <string>
#include <memory>
#include <SDL_ttf.h>
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* owner);


		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<Font> font);
		void SetColor(const SDL_Color& color);

		std::shared_ptr<Texture2D> GetTexture();



	private:
		void SetTexture();
		std::string m_Text{ "default text" };
		std::shared_ptr<Font> m_Font;

		SDL_Color m_Color{ 255,255,255 };

		bool m_NeedsUpdate{ false };

		std::shared_ptr<Texture2D> m_Texture;
	};
}
