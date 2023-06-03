#include <cassert>
#include "Component.h"

namespace dae {

	Component::Component(GameObject* owner)
	{
		m_Owner = owner;
	}

	GameObject* Component::GetOwner() const
	{
		return m_Owner;
	}
}