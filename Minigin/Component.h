#pragma once
#include <memory>

#include "GameObject.h"
namespace dae
{
	class GameObject;

	class Component
	{

	public:

		Component(std::weak_ptr<GameObject> owner);

		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;


		void SetOwner(std::weak_ptr<GameObject> owner);



	protected:

		std::weak_ptr<GameObject> m_Owner;
	};

}

