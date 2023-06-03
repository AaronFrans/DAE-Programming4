#pragma once
#include "Component.h"

namespace dae {

	class ImageComponent;
	class TransformComponent;

	class ImageRenderComponent final :
		public Component
	{
	public:
		ImageRenderComponent(GameObject* owner);

		~ImageRenderComponent() = default;
		ImageRenderComponent(const ImageRenderComponent& other) = delete;
		ImageRenderComponent(ImageRenderComponent&& other) = delete;
		ImageRenderComponent& operator=(const ImageRenderComponent& other) = delete;
		ImageRenderComponent& operator=(ImageRenderComponent&& other) = delete;


		void Render() const override;

	private:
		void CheckForRequiredComponents() const;
		void SetupRequiredComponents();

		ImageComponent* m_Image{};
		TransformComponent* m_Transform{};

	};
}


