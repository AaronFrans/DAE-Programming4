#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include "glm/glm.hpp"

namespace dae
{

	class Texture2D;

	class ImageComponent final : public Component
	{
	public:
		ImageComponent(std::weak_ptr<GameObject> owner);


		virtual ~ImageComponent() = default;
		ImageComponent(const ImageComponent& other) = delete;
		ImageComponent(ImageComponent&& other) = delete;
		ImageComponent& operator=(const ImageComponent& other) = delete;
		ImageComponent& operator=(ImageComponent&& other) = delete;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D>& texture);

		float GetTextureWidth() const;
		float GetTextureHeight() const;

		glm::vec2 GetTextureDimensions() const;
		std::shared_ptr<Texture2D> GetTexture() const;

	private:
		std::shared_ptr<Texture2D> m_Texture;
	};
}

