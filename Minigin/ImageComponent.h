#pragma once
#include <string>
#include <memory>
#include "Component.h"

namespace dae
{

	class Texture2D;

	class ImageComponent : public Component
	{
	public:
		ImageComponent(std::weak_ptr<GameObject> owner);


		virtual ~ImageComponent() = default;
		ImageComponent(const ImageComponent& other) = delete;
		ImageComponent(ImageComponent&& other) = delete;
		ImageComponent& operator=(const ImageComponent& other) = delete;
		ImageComponent& operator=(ImageComponent&& other) = delete;

		void SetTexture(const std::string& filename);
		std::shared_ptr<Texture2D> GetTexture() const;

	private:
		std::shared_ptr<Texture2D> m_Texture;
	};
}

