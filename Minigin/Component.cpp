#include "Component.h"

namespace dae {

	Component::Component()
	{
	}

	void Component::SetOwner(std::weak_ptr<GameObject> owner)
	{
		m_Owner = owner;
	}
}