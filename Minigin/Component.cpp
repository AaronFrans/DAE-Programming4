#include <cassert>
#include "Component.h"

namespace dae {

	Component::Component(std::weak_ptr<GameObject> owner)
	{
		m_Owner = owner;
	}

	void Component::SetOwner(std::weak_ptr<GameObject> owner)
	{
		assert(m_Owner.expired() && "Owner already set for component");

		m_Owner = owner;
	}
}