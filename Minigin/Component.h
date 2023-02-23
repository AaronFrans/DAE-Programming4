#pragma once
#include <memory>

#include "GameObject.h"
namespace dae
{
	class GameObject;

	class Component
	{

	public:

		Component();

		virtual ~Component() {};



		virtual void Render() const {};
		virtual void Update() {};

		void SetOwner(std::weak_ptr<GameObject> owner);



	protected:

		std::weak_ptr<GameObject> m_Owner;
	};

}

