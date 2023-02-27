#include "RenderComponent.h"


dae::RenderComponent::RenderComponent(std::weak_ptr<GameObject> owner)
	:Component(owner)
{
}
