#pragma once
#include "Component.h"

namespace dae
{
	class RenderComponent :
		public Component
	{

	public:
		RenderComponent(std::weak_ptr<GameObject> owner);

		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		virtual void Render() const {};
	};

}

