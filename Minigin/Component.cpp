#include <cassert>
#include "Component.h"

namespace dae {

	Component::Component(std::weak_ptr<GameObject> owner)
	{
		m_Owner = owner;
	}
}