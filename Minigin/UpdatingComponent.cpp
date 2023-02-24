#include "UpdatingComponent.h"

dae::UpdatingComponent::UpdatingComponent()
	: Component()
{}

dae::UpdatingComponent::UpdatingComponent(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}
