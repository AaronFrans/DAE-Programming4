#pragma once
#include "Component.h"

namespace dae {

	class ImageComponent;
	class TransformComponent;

	class ImageRenderComponent final :
		public Component
	{
	public:
		ImageRenderComponent(std::weak_ptr<GameObject> owner);

		~ImageRenderComponent() = default;
		ImageRenderComponent(const ImageRenderComponent& other) = delete;
		ImageRenderComponent(ImageRenderComponent&& other) = delete;
		ImageRenderComponent& operator=(const ImageRenderComponent& other) = delete;
		ImageRenderComponent& operator=(ImageRenderComponent&& other) = delete;


		void Render() const override;

	private:
		void CheckForRequiredComponents() const;
		void SetupRequiredComponents();

		std::weak_ptr<ImageComponent> m_Image;
		std::weak_ptr<TransformComponent> m_Transform;

	};
}


