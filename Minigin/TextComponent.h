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
		TextComponent(std::weak_ptr<GameObject> owner);


		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

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
